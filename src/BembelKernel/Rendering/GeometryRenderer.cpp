/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "GeometryRenderer.h"

#include "Shader.h"
#include "GeometryModel.h"
#include "GeometryMesh.h"

#include <glm/glm.hpp>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

GeometryRenderer::GeometryRenderer()
{}

GeometryRenderer::~GeometryRenderer()
{}

void GeometryRenderer::SetAssetMannager(std::shared_ptr<AssetManager> assetMgr)
{
	_assetMgr = assetMgr;
}

void GeometryRenderer::SetShader(std::shared_ptr<Shader> shader)
{
	_shader = shader;
}

void GeometryRenderer::DrawGeometry(
	const glm::mat4& view, 
	const glm::mat4& proj, 
	const std::vector<GeometryInstance>& instances)
{
	if (!_shader || !_assetMgr)
		return;

	s_renderData.clear();
	s_renderData.reserve(instances.size());

	for (auto& it : instances)
	{
		glm::mat4 transform = view*it.transformation;

		GeometryModel* model = _assetMgr->GetAsset<GeometryModel>(it.model);
		if (!model)
			continue;

		GeometryMesh* mesh = _assetMgr->GetAsset<GeometryMesh>(model->GetMesh());
		if (!mesh)
			continue;

		mesh->Bind();
		for (const auto& matMapping : model->GetMateialMapping())
		{
			Material* mat = _assetMgr->GetAsset<Material>(matMapping.material);
			if(!mat)
				continue;

			unsigned first, count;
			if (!mesh->GetSubMesh(matMapping.subMesh, first, count))
				continue;

			s_renderData.push_back(RenderData{
				mesh, mat, first, count, transform
			});
		}
	}
	DoRendering(proj);
}

std::shared_ptr<GeometryRenderer> GeometryRenderer::CreateRenderer(
	const xml::Element* properties)
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
		auto renderer = std::make_shared<GeometryRenderer>();
		renderer->SetShader(program);
		return renderer;
	}
	return nullptr;
}

void GeometryRenderer::DoRendering(const glm::mat4& proj)
{
	std::sort(s_renderData.begin(), s_renderData.end(), [](
		const RenderData& first, const RenderData& second
	){
		return (first.mesh != second.mesh) ? 
			first.mesh < second.mesh :
			first.material < second.material;
	});

	_shader->Use();
	glUniformMatrix4fv(
		_shader->GetUniformLocation("uProjectionMatrix"),
		1, GL_FALSE, &proj[0][0]);

	GeometryMesh* currentMesh = nullptr;
	Material*     currentMaterial = nullptr;
	for (size_t n = 0; n < s_renderData.size(); ++n)
	{
		if (s_renderData[n].mesh != currentMesh)
		{
			currentMesh = s_renderData[n].mesh;
			currentMesh->Bind();
		}
		if (s_renderData[n].material != currentMaterial)
		{
			currentMaterial = s_renderData[n].material;
			UseMaterial(currentMaterial);
		}

		glUniformMatrix4fv(
			_shader->GetUniformLocation("uModleViewMatrix"),
			1, GL_FALSE, &s_renderData[n].transform[0][0]);
		glUniformMatrix4fv(
			_shader->GetUniformLocation("uNormalMatrix"),
			1, GL_FALSE, &s_renderData[n].transform[0][0]);

		glDrawElements(
			GL_TRIANGLES,
			s_renderData[n].count,
			GL_UNSIGNED_INT,
			(void*)(s_renderData[n].first * sizeof(unsigned))
		);
	}
}

std::vector<GeometryRenderer::RenderData> GeometryRenderer::s_renderData;

void GeometryRenderer::UseMaterial(Material* material)
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
