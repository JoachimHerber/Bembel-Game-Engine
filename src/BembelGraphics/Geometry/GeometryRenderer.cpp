/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "GeometryRenderer.h"

#include <BembelKernel/Assets/AssetManager.h>
#include <BembelKernel/Renderig/Texture.h>
#include <BembelKernel/Renderig/Shader.h>
#include <BembelKernel/Renderig/GeometryMesh.h>

#include <BembelKernel/Renderig/Material.h>
#include <BembelKernel/Renderig/GeometryModel.h>

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


void main()
{
	vNormal   = normalize((uNormalMatrix*vec4(aNormal,0)).xyz);
	vTexCoord = aTexCoord;

	gl_Position = 
		uProjectionMatrix*uModleViewMatrix*vec4(aPosition,1);
}
)";

const static std::string frag = R"(
#version 330

in vec3 vNormal;
in vec2 vTexCoord;

uniform vec3 uAlbedo;
uniform vec3 uEmission;
uniform vec3 uReflectivity;
uniform float uRoughness;

layout(location = 0) out vec3 oEmission;
layout(location = 1) out vec3 oAlbedo;
layout(location = 2) out vec3 oReflectivity;
layout(location = 3) out vec4 oNormal;

void main()
{
	oEmission     = uEmission;
	oAlbedo       = uAlbedo;
	oReflectivity = uReflectivity;
	oNormal       = vec4(0.5*normalize(vNormal) + vec3(0.5), uRoughness);
}
)";
	_geometryPassShader = std::make_unique<Shader>();
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

	auto assetMgr = _scene->GetAssetManager();

	for (const auto& it : visibleInstances)
	{
		GeometryModel* model = assetMgr->GetAsset<GeometryModel>(it.first);
		if(!model)
			continue;
		
		GeometryMesh* mesh = assetMgr->GetAsset<GeometryMesh>(model->GetMesh());
		if(!mesh)
			continue;

		mesh->Bind();
		for (const auto& matMapping : model->GetMateialMapping())
		{
			unsigned first, count;
			if(!mesh->GetSubMesh(matMapping.subMesh, first, count))
				continue;

			if (!shadowPass)
				UseMaterial(matMapping.material);

			for (const auto& instance : it.second)
			{
				glUniformMatrix4fv(
					shader->GetUniformLocation("uModleViewMatrix"),
					1, GL_FALSE, &instance[0][0]);
				glUniformMatrix4fv(
					shader->GetUniformLocation("uNormalMatrix"),
					1, GL_FALSE, &instance[0][0]);
				glDrawElements(
					GL_TRIANGLES,
					count, 
					GL_UNSIGNED_INT, 
					(void*)(first)
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
	auto assetMgr = _scene->GetAssetManager();

	Material* material = assetMgr->GetAsset<Material>(materialHandle);

	if (!material)
		return false;

	glUniform3fv(
		_geometryPassShader->GetUniformLocation("uAlbedo"),
		1, &(material->GetAlbedo()[0]));
	glUniform3fv(
		_geometryPassShader->GetUniformLocation("uEmission"),
		1, &(material->GetEmission()[0]));
	glUniform3fv(
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
	Texture* texture = _scene->GetAssetManager()->GetAsset<Texture>(textureHandle, false);
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
