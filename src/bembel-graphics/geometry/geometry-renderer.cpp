/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "geometry-renderer.h"

#include "geometry-model.h"
#include "geometry-mesh.h"

#include <iostream>

#include <glm/glm.hpp>

#include <bembel-kernel/rendering/shader.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

DefaultGeometryRenderer::DefaultGeometryRenderer(
	AssetManager* asset_manager, unsigned id)
	: GeometryRendererBase(asset_manager, id)
{}

DefaultGeometryRenderer::~DefaultGeometryRenderer()
{}

bool DefaultGeometryRenderer::SetShader(AssetHandle shader)
{
	auto shader_pointer = asset_manager_->GetAsset<ShaderProgram>(shader);
	if( shader_pointer == nullptr )
		return false;

	shader_ = shader;

	material_uniform_block_index_ =
		shader_pointer->GetUniformBlockIndex("Material");
	material_uniform_buffer_size_ =
		shader_pointer->GetUniformBlockDataSize(material_uniform_block_index_);
}

void DefaultGeometryRenderer::Render(
	const glm::mat4& proj,
	const glm::mat4& view,
	const std::vector<GeometryRenderData>& data)
{

	auto shader_pointer = asset_manager_->GetAsset<ShaderProgram>(shader_);

	if( !shader_pointer )
		return;

	shader_pointer->Use();

	glUniformMatrix4fv(
		shader_pointer->GetUniformLocation("uProjectionMatrix"),
		1, GL_FALSE, &proj[0][0]);

	GeometryMesh* currentMesh = nullptr;
	Material*     currentMaterial = nullptr;
	for( const auto& it : data )
	{
		if( !it.mesh || !it.material )
			continue;

		if( it.mesh != currentMesh )
		{
			currentMesh = it.mesh;
			glBindVertexArray(currentMesh->GetVAO());
		}
		if( it.material != currentMaterial )
		{
			currentMaterial = it.material;
			glBindBufferBase(
				GL_UNIFORM_BUFFER, 0, currentMaterial->GetUniformBufferObject());
		}

		glm::mat4 modleView = view*it.transform;

		glUniformMatrix4fv(
			shader_pointer->GetUniformLocation("uModleViewMatrix"),
			1, GL_FALSE, &(modleView[0][0]));
		glUniformMatrix4fv(
			shader_pointer->GetUniformLocation("uNormalMatrix"),
			1, GL_FALSE, &(modleView[0][0]));

		glLoadIdentity();
		glMultMatrixf(&(it.transform[0][0]));
		glDrawElements(
			GL_TRIANGLES,
			it.count,
			GL_UNSIGNED_INT,
			(void*) (it.first * sizeof(unsigned))
		);
	}

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindVertexArray(0);
}


std::unique_ptr<Material> DefaultGeometryRenderer::CreateMaterial(
	const xml::Element* properties)
{
	auto mat = std::make_unique<Material>(
		GetRendererID(), material_uniform_buffer_size_);

	glm::vec3 albedo = {0.9f, 0.9f, 0.1f};
	glm::vec3 emission = {0.0f, 0.0f, 0.0f};
	glm::vec3 reflectivity = {0.2f, 0.2f, 0.2f};
	float     roughness = {0.5f};
	xml::GetAttribute(properties, "albedo", albedo);
	xml::GetAttribute(properties, "emission", emission);
	xml::GetAttribute(properties, "reflectivity", reflectivity);
	xml::GetAttribute(properties, "roughness", roughness);

	float matData[] = {
		albedo.r, albedo.g, albedo.b, 1.0f,
		emission.r, emission.g, emission.b,  1.0f,
		reflectivity.r, reflectivity.g, reflectivity.b, 1.0f,
		roughness};

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, mat->GetUniformBufferObject());
	glBufferSubData(
		GL_UNIFORM_BUFFER, 0,
		13*sizeof(float), &matData
	);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return std::move(mat);
}

std::unique_ptr<DefaultGeometryRenderer>
DefaultGeometryRenderer::CreateRenderer(
	const xml::Element* properties, AssetManager* asset_manager, unsigned id)
{
	auto program = asset_manager->RequestAsset<ShaderProgram>(
		properties->FirstChildElement("ShaderProgram"));
	if( asset_manager->IsHandelValid(program) )
	{
		auto renderer = std::make_unique<DefaultGeometryRenderer>(
			asset_manager,id);
		renderer->SetShader(program);
		return std::move(renderer);
	}
	return nullptr;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
