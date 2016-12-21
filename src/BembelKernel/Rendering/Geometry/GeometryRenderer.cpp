/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "GeometryRenderer.h"

#include "../Shader.h"
#include "GeometryModel.h"
#include "GeometryMesh.h"

#include <glm/glm.hpp>

#include <iostream>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

DefaultGeometryRenderer::DefaultGeometryRenderer(unsigned id)
	: GeometryRendererBase(id)
{}

DefaultGeometryRenderer::~DefaultGeometryRenderer()
{}

void DefaultGeometryRenderer::SetShader(std::shared_ptr<Shader> shader)
{
	_shader = shader;
}

void DefaultGeometryRenderer::Render(
	const glm::mat4& proj,
	const glm::mat4& view,
	const std::vector<GeometryRenderData>& data)
{

	if (!_shader)
		return;

	_shader->Use(),
	glUniformMatrix4fv(
		_shader->GetUniformLocation("uProjectionMatrix"),
		1, GL_FALSE, &proj[0][0]);

	GeometryMesh* currentMesh = nullptr;
	Material*     currentMaterial = nullptr;
	for (const auto& it : data)
	{
		if (!it.mesh || !it.material)
			continue;

		if (it.mesh != currentMesh)
		{
			currentMesh = it.mesh;
			glBindVertexArray(currentMesh->GetVAO());
		}
		if (it.material != currentMaterial)
		{
			currentMaterial = it.material;
			UseMaterial(currentMaterial);
		}

		glm::mat4 modleView = view*it.transform;

		glUniformMatrix4fv(
			_shader->GetUniformLocation("uModleViewMatrix"),
			1, GL_FALSE, &(modleView[0][0]));
		glUniformMatrix4fv(
			_shader->GetUniformLocation("uNormalMatrix"),
			1, GL_FALSE, &(modleView[0][0]));

		glLoadIdentity();
		glMultMatrixf(&(it.transform[0][0]));
		glDrawElements(
			GL_TRIANGLES,
			it.count,
			GL_UNSIGNED_INT,
			(void*)(it.first * sizeof(unsigned))
		);
	}

	glBindVertexArray(0);
}

std::unique_ptr<DefaultGeometryRenderer>
	DefaultGeometryRenderer::CreateRenderer(
		const xml::Element* properties, unsigned id)
{
	std::string filename;

	auto program = std::make_shared<Shader>();
	for (auto shader : xml::IterateChildElements(properties, "VertexShader"))
	{
		if (xml::GetAttribute(shader, "file", filename))
			program->AttachShaderFromFile(GL_VERTEX_SHADER, filename);
	}
	for (auto shader : xml::IterateChildElements(properties, "FragmentShader"))
	{
		if (xml::GetAttribute(shader, "file", filename))
			program->AttachShaderFromFile(GL_FRAGMENT_SHADER, filename);
	}

	if (program->Link())
	{
		auto renderer = std::make_unique<DefaultGeometryRenderer>(id);
		renderer->SetShader(program);
		return std::move(renderer);
	}
	return nullptr;
}

void DefaultGeometryRenderer::UseMaterial(Material* material)
{
	glUniform3fv(
		_shader->GetUniformLocation("uAlbedo"),
		1, &(material->GetAlbedo()[0]));
	glUniform3fv(
		_shader->GetUniformLocation("uEmission"),
		1, &(material->GetEmission()[0]));
	glUniform3fv(
		_shader->GetUniformLocation("uReflectivity"),
		1, &(material->GetReflectivity()[0]));
	glUniform1f(
		_shader->GetUniformLocation("uRoughness"),
		material->GetRoughness());
	glUniform1i(
		_shader->GetUniformLocation("uNormalMapHasRoughness"),
		material->GetNormalMapHasRoughness());

// 	ApplyTexture(GL_TEXTURE0, material->GetNormalTexture(),       "uHasNormalTexture");
// 	ApplyTexture(GL_TEXTURE1, material->GetAlbedoTexture(),       "uHasAlbedoTexture");
// 	ApplyTexture(GL_TEXTURE2, material->GetEmissionTexture(),     "uHasEmissionTexture");
// 	ApplyTexture(GL_TEXTURE3, material->GetReflectivityTexture(), "uHasReflectivityTexture");
}

// void GeometryRenderer::ApplyTexture(
// 	GLenum textureUnit,
// 	AssetHandle textureHandle, 
// 	const std::string& uniform)
// {
// 	Texture* texture = _scene->GetAssetManager()->GetAsset<Texture>(textureHandle, false);
// 	if (texture)
// 	{
// 		glActiveTexture(textureUnit);
// 		texture->Bind();
// 		glUniform1i(_geometryPassShader->GetUniformLocation(uniform), 1);
// 	}
// 	else
// 	{
// 		glUniform1i(_geometryPassShader->GetUniformLocation(uniform), 0);
// 	}
// }

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
