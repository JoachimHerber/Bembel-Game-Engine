/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "GeometryRenderer.h"

#include <BembelKernel/Assets/AssetManager.h>
#include <BembelGraphics/OpenGL/Texture.h>
#include <BembelGraphics/OpenGL/ShaderProgram.h>
#include <BembelGraphics/OpenGL/VertexArrayObject.h>

#include "Material.h"
#include "GeometryModel.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

GeometryRenderer::GeometryRenderer()
{}

GeometryRenderer::~GeometryRenderer()
{}

void GeometryRenderer::Init()
{

const static std::string vert = R"(
#version 330
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;  
layout(location = 2) in vec2 aTexCoord;  

uniform mat4 uNormalMatrix;
uniform mat4 uModleViewMatrix;
uniform mat4 uProjectionMatrix;

out vec3 vNormal;
out vec2 vTexCoord;

const vec3 gVertices[3] = vec3[](
vec3(+0.0000,+2.5000,-0), 
vec3(-1.0000,+0.0000,+0), 
vec3(+1.0000,+0.0000,+0)
);

void main()
{
	vNormal   = normalize((uNormalMatrix*vec4(aNormal,0)).xyz);
	vTexCoord = aTexCoord;

	gl_Position = 
		uProjectionMatrix*uModleViewMatrix*vec4(aPosition,1);
	//gl_Position = 
	//	uProjectionMatrix*uModleViewMatrix*vec4(gVertices[gl_VertexID%3],1);
}
)";

const static std::string frag = R"(
#version 330

in vec3 vNormal;
in vec2 vTexCoord;

layout(location = 0) out vec3 oEmission;
layout(location = 1) out vec3 oAlbedo;
layout(location = 2) out vec3 oReflectivity;
layout(location = 3) out vec4 oNormal;

void main()
{
	oEmission     = vec3(0.0, 0.0, 0.0);
	oAlbedo       = vec3(1.0, 0.0, 0.0);
	oReflectivity = vec3(0.5, 0.5, 0.5);
	oNormal       = vec4(0.5, 0.5, 1.0, 0.2);
}
)";
	_geometryPassShader = std::make_unique<ShaderProgram>();
	_geometryPassShader->AttachShader(GL_VERTEX_SHADER, vert);
	_geometryPassShader->AttachShader(GL_FRAGMENT_SHADER, frag);
	_geometryPassShader->Link();
	_geometryPassShader->Use();
	glUniform1i(_geometryPassShader->GetUniformLocation("uNormalTexture"), 0);
	glUniform1i(_geometryPassShader->GetUniformLocation("uAlbedoTexture"), 1);
	glUniform1i(_geometryPassShader->GetUniformLocation("uEmissionTexture"), 2);
	glUniform1i(_geometryPassShader->GetUniformLocation("uReflectivityTexture"), 3);
}

void GeometryRenderer::Cleanup()
{

}

void GeometryRenderer::SetScene(ScenePtr scene)
{
	_scene = scene;
	_geometryContainer = scene->RequestComponentContainer<GeometryInstance>();
	_positionConteiner = scene->RequestComponentContainer<PositionComponent>();

	auto assetMgr = scene->GetAssetManager();
	_models    = assetMgr->RequestAssetContainer<GeometryModel>();
	_materials = assetMgr->RequestAssetContainer<Material>();
	_textures  = assetMgr->RequestAssetContainer<Texture>();
}

void GeometryRenderer::DoGeometryPass(
	const glm::mat4& view, 
	const glm::mat4& projection, 
	const ViewFrustum& frustum)
{
	DrawGeometry(view, projection, frustum, false);
}

void GeometryRenderer::DoShadowPass(
	const glm::mat4& view, 
	const glm::mat4& projection, 
	const ViewFrustum& frustum)
{
	DrawGeometry(view, projection, frustum, true);
}

std::unique_ptr<GeometryRenderer> GeometryRenderer::CreateInstance(const xml::Element* properties)
{
	auto renderer = std::make_unique<GeometryRenderer>();

	return std::move(renderer);
}

void GeometryRenderer::DrawGeometry(
	const glm::mat4& view, 
	const glm::mat4& projection, 
	const ViewFrustum& frustum, 
	bool shadowPass)
{
	auto shader = shadowPass ? _shadowPassShader.get() : _geometryPassShader.get();

	if (!shader)
		return;

	VisibleGeometry visibleInstances;
	GetEntitysWhithinViewFrustum(view, frustum, visibleInstances, shadowPass);
	
	shader->Use();

	glUniformMatrix4fv(
		shader->GetUniformLocation("uProjectionMatrix"),
		1, GL_FALSE, &projection[0][0]);

	for (const auto& it : visibleInstances)
	{
		GeometryModel* model = _models->GetAsset(it.first);
		if(!model)
			continue;

		model->GetVAO()->Bind();
		for (const auto& subMesh : model->GetSubMeshes())
		{
			if (!shadowPass)
				UseMaterial(subMesh.material);

			for (const auto& instance : it.second)
			{
				glUniformMatrix4fv(
					shader->GetUniformLocation("uModleViewMatrix"),
					1, GL_FALSE, &instance[0][0]);
				glDrawElements(
					subMesh.primitiveType,
					subMesh.numIndices, 
					GL_UNSIGNED_INT, 
					(void*)(subMesh.firstIndex)
				);

// 				glPointSize(5);
// 				glDrawArrays(GL_POINTS, subMesh.firstIndex, subMesh.numIndices);
			}
		}
	}
}

void GeometryRenderer::GetEntitysWhithinViewFrustum(
	const glm::mat4& view, 
	const ViewFrustum& frustum, 
	VisibleGeometry& visibleInstances,
	bool shadowCasterOnly)
{
	Scene::ComponentMask mask =
		_geometryContainer->GetComponentMask() |
		_positionConteiner->GetComponentMask();

	const auto& positions         = _positionConteiner->GetComponents();
	const auto& geometryInstances = _geometryContainer->GetComponents();
	const auto& entityMasks = _scene->GetEntitys();

	for (Scene::EntityID entity = 0; entity<entityMasks.size(); ++entity)
	{
		if((entityMasks[entity] & mask) != mask)
			continue;

		if(shadowCasterOnly && !geometryInstances[entity].isCastingShadow)
			continue;

		const glm::vec3&  pos   = positions[entity].position;
		const AssetHandle model = geometryInstances[entity].model;

		glm::mat4 modleView = view*glm::translate(pos);
		visibleInstances[model].push_back(modleView);
	}
}

bool GeometryRenderer::UseMaterial(AssetHandle materialHandle )
{
	Material* material = _materials->GetAsset(materialHandle);

	if (!material)
		return false;

	glUniform4fv(
		_geometryPassShader->GetUniformLocation("uAlbedo"),
		1, &(material->GetAlbedo()[0]));
	glUniform4fv(
		_geometryPassShader->GetUniformLocation("uEmission"),
		1, &(material->GetEmission()[0]));
	glUniform4fv(
		_geometryPassShader->GetUniformLocation("uReflectivity"),
		1, &(material->GetReflectivity()[0]));
	glUniform1f(
		_geometryPassShader->GetUniformLocation("uRoughness"),
		material->GetRoughness());
	glUniform1i(
		_geometryPassShader->GetUniformLocation("uNormalMapHasRoughness"),
		material->GetNormalMapHasRoughness());

	ApplyTexture(GL_TEXTURE0, material->GetNormalTexture(),       "uHasNormalTexture");
	ApplyTexture(GL_TEXTURE1, material->GetAlbedoTexture(),       "uHasAlbedoTexture");
	ApplyTexture(GL_TEXTURE2, material->GetEmissionTexture(),     "uHasEmissionTexture");
	ApplyTexture(GL_TEXTURE3, material->GetReflectivityTexture(), "uHasReflectivityTexture");
	return true;
}

void GeometryRenderer::ApplyTexture(
	GLenum textureUnit,
	AssetHandle textureHandle, 
	const std::string& uniform)
{
	Texture* texture = _textures->GetAsset(textureHandle, false);
	if (texture)
	{
		glActiveTexture(textureUnit);
		texture->Bind();
		glUniform1i(_geometryPassShader->GetUniformLocation(uniform), 1);
	}
	else
	{
		glUniform1i(_geometryPassShader->GetUniformLocation(uniform), 0);
	}
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
