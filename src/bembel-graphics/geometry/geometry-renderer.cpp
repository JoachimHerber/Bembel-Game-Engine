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

DefaultGeometryRenderer::DefaultGeometryRenderer(unsigned id)
	: GeometryRendererBase(id)
{}

DefaultGeometryRenderer::~DefaultGeometryRenderer()
{}

void DefaultGeometryRenderer::SetShader(std::shared_ptr<Shader> shader)
{
	_shader = shader;

	_materialUniformBlockIndex = 
		_shader->GetUniformBlockIndex("Material");
	_materialUniformBufferSize =
		_shader->GetUniformBlockDataSize( _materialUniformBlockIndex );
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
			glBindBufferBase( 
				GL_UNIFORM_BUFFER, 0, currentMaterial->GetUniformBufferObject() );
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

	glBindBuffer( GL_UNIFORM_BUFFER, 0 );
	glBindVertexArray(0);
}


std::unique_ptr<Material> DefaultGeometryRenderer::CreateMaterial( 
	const xml::Element* properties )
{
	auto mat = std::make_unique<Material>(GetRendererID(), _materialUniformBufferSize);
	
	glm::vec3 albedo = {0.9f, 0.9f, 0.1f};
	glm::vec3 emission = {0.0f, 0.0f, 0.0f};
	glm::vec3 reflectivity = {0.2f, 0.2f, 0.2f};
	float     roughness = {0.5f};
	xml::GetAttribute( properties, "albedo", albedo );
	xml::GetAttribute( properties, "emission", emission );
	xml::GetAttribute( properties, "reflectivity", reflectivity );
	xml::GetAttribute( properties, "roughness", roughness );
	
	float matData[] = {
		albedo.r, albedo.g, albedo.b, 1.0f,
		emission.r, emission.g, emission.b,  1.0f,
		reflectivity.r, reflectivity.g, reflectivity.b, 1.0f,
		roughness};

	glBindBufferBase( GL_UNIFORM_BUFFER, 0, mat->GetUniformBufferObject() );
	glBufferSubData( 
		GL_UNIFORM_BUFFER,  0, 
		13*sizeof(float), &matData
	);
	glBindBuffer( GL_UNIFORM_BUFFER, 0 );

	return std::move(mat);
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

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
