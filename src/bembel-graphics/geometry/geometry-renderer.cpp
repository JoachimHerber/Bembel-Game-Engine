/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2018 Joachim Herber                                     * */
/* *                                                                        * */
/* *   Permission is hereby granted, free of charge, to any person          * */
/* *   obtaining copy of this software and associated documentation files   * */
/* *   (the "Software"), to deal in the Software without restriction,       * */
/* *   including without limitation the rights to use, copy, modify, merge, * */
/* *   publish, distribute, sublicense, and/or sell copies of the Software, * */
/* *   and to permit persons to whom the Software is furnished to do so,    * */
/* *   subject to the following conditions :                                * */
/* *                                                                        * */
/* *   The above copyright notice and this permission notice shall be       * */
/* *   included in all copies or substantial portions of the Software.      * */
/* *                                                                        * */
/* *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      * */
/* *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   * */
/* *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                * */
/* *   NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   * */
/* *   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   * */
/* *   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    * */
/* *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     * */
/* *   SOFTWARE.                                                            * */
/* *                                                                        * */
/* ************************************************************************** */
/******************************************************************************/

/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "geometry-renderer.h"

#include "geometry-model.h"
#include "geometry-mesh.h"

#include <iostream>

#include <glm/glm.hpp>

#include <bembel-kernel/rendering/shader.h>
#include <bembel-kernel/rendering/texture.h>

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

void DefaultGeometryRenderer::AddRequiredTexture(
	const std::string & texture_name,
	const std::string & uniform_sampler_name)
{
	required_textures_.push_back({texture_name, uniform_sampler_name});
}

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

	std::vector<GLint> active_uniform_indices;
	shader_pointer->GetUniformBlockActiveUniformIndices(
		material_uniform_block_index_, &active_uniform_indices);

	for( GLint uniform_index : active_uniform_indices )
	{
		MaterialParam param;
		param.offset = shader_pointer->GetActiveUniformOffset(uniform_index);
		std::string name;
		GLint size;
		shader_pointer->GetActiveUniform(uniform_index, &param.size, &param.type, &name);

		material_params_[name] = param;
		std::string param_name = name.substr(name.find_last_of(".") + 1);
		material_params_[param_name] = param;
	}

	shader_pointer->Use();
	GLuint sampler_location = 0;
	for( const auto& it : required_textures_ )
	{
		glUniform1i(
			shader_pointer->GetUniformLocation(it.uniform_sampler_name),
			sampler_location);
		++sampler_location;
	}

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

			GLenum active_texture = GL_TEXTURE0;
			for(auto it : currentMaterial->GetTextures())
			{
				glActiveTexture(active_texture);
				auto texture = asset_manager_->GetAsset<Texture>(it);
				texture->Bind();
				active_texture = active_texture + 1;
			}
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


namespace {
template<typename T>
bool ReadMaterialParam(
	const xml::Element* properties,
	const std::string& param_name,
	GLbyte* dest)
{
	T value;
	if( xml::GetAttribute(properties, param_name, value) )
	{
		memcpy(dest, &value, sizeof(value));
		return true;
	}
	return false;
}
}//anon namespace

bool DefaultGeometryRenderer::ReadMaterialParameter(
	const xml::Element* properties,
	const std::string& param_name,
	const MaterialParam& param,
	GLbyte* buffer)
{
	switch( param.type )
	{
	case GL_FLOAT:
		return ReadMaterialParam<float>(properties, param_name, buffer + param.offset);
	case GL_FLOAT_VEC2:
		return ReadMaterialParam<glm::vec2>(properties, param_name, buffer + param.offset);
	case GL_FLOAT_VEC3:
		return ReadMaterialParam<glm::vec3>(properties, param_name, buffer + param.offset);
	case GL_FLOAT_VEC4:
		return ReadMaterialParam<glm::vec4>(properties, param_name, buffer + param.offset);
	}
	return false;
}

std::unique_ptr<Material> DefaultGeometryRenderer::CreateMaterial(
	AssetManager* asset_manager,
	const xml::Element* properties)
{
	auto mat = std::make_unique<Material>(
		GetRendererID(), material_uniform_buffer_size_);

	std::vector<AssetHandle> textures;
	for( auto& it : required_textures_ )
	{
		auto texture = properties->FirstChildElement(it.texture_name.c_str());
		if( texture == nullptr )
		{
			BEMBEL_LOG_ERROR() << "Material does not secify a '"<< it.texture_name <<"' texture.";
			return nullptr;
		}
		
		auto texture_hndl_ = asset_manager->RequestAsset<Texture>(texture);
		if(!asset_manager->IsHandelValid(texture_hndl_))
		{
			BEMBEL_LOG_ERROR() << "Can't find reqired '"<< it.texture_name <<"' texture for material.";
			return nullptr;
		}
		textures.push_back(texture_hndl_);
	}
	mat->SetTextures(textures);

	std::vector<GLbyte> data;
	data.resize(material_uniform_buffer_size_);
	for( auto& it : material_params_ )
	{
		ReadMaterialParameter(properties, it.first, it.second, &data[0]);
	}

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, mat->GetUniformBufferObject());
	glBufferSubData(GL_UNIFORM_BUFFER, 0, material_uniform_buffer_size_, &data[0]);
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
			asset_manager, id);

		for( auto it : xml::IterateChildElements(properties, "RequiredTexture") )
		{
			std::string  texture_name, sampler_uniform;
			xml::GetAttribute(it, "texturen_name", texture_name);
			xml::GetAttribute(it, "sampler_uniform_name", sampler_uniform);

			renderer->AddRequiredTexture(texture_name, sampler_uniform);
		}

		if( renderer->SetShader(program) )
			return std::move(renderer);
	}
	return nullptr;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
