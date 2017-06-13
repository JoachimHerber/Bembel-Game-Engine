/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2017 Joachim Herber                                     * */
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

#include "shader.h"

#include <fstream>

#include <bembel-base/logging/logger.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

Shader::Shader(GLuint handle)
	: handle_(handle)
{}

Shader::~Shader()
{
	glDeleteShader(handle_);
}

bool Shader::Init(const std::string& source)
{
	const char* c_str = source.c_str();
	const int   length = static_cast<int>(source.length());
	glShaderSource(handle_, 1, &c_str, &length);
	glCompileShader(handle_);

	int status;
	glGetShaderiv(handle_, GL_COMPILE_STATUS, &status);
	if( status != int(GL_TRUE) )
	{
		char error_message[4096];
		GLsizei size;
		glGetShaderInfoLog(
			handle_, 4096, &size, error_message);

		BEMBEL_LOG_ERROR()
			<< "Can't compile Shader" << std::endl
			<< error_message << std::endl;

		return false;
	}
	return true;
}


GLuint Shader::GetHandle() const
{
	return handle_;
}

const std::string& Shader::GetTypeName()
{
	const static std::string type_name = "Shader";
	return type_name;
}

std::unique_ptr<Shader> Shader::LoadAsset(
	AssetManager* asset_manager, const std::string& file_name)
{
	xml::Document doc;
	if( doc.LoadFile(file_name.c_str()) != tinyxml2::XML_SUCCESS )
	{
		BEMBEL_LOG_ERROR()
			<< "Failed to load file '" << file_name << "'\n"
			<< doc.ErrorName() << std::endl;
		return nullptr;
	}

	const xml::Element* root = doc.FirstChildElement(GetTypeName().c_str());
	if( !root )
	{
		BEMBEL_LOG_ERROR()
			<< "File '" << file_name << "' has no root element 'Shader'"
			<< std::endl;
		return nullptr;
	}
	return CreateAsset(asset_manager, root);
}

std::unique_ptr<Shader> Shader::CreateAsset(
	AssetManager* asset_manager, const xml::Element* properties)
{
	const std::map<std::string, GLenum> shader_type_map = {
		{"GL_VERTEX_SHADER", GL_VERTEX_SHADER},
		{"GL_FRAGMENT_SHADER", GL_FRAGMENT_SHADER},
		{"GL_GEOMETRY_SHADER", GL_GEOMETRY_SHADER},
	};

	if( properties->GetText() == nullptr)
		return nullptr;

	std::string type;
	if( !xml::GetAttribute(properties, "type", type) )
		return nullptr;

	auto it = shader_type_map.find(type);
	if( it != shader_type_map.end() )
		return CreateShader(it->second, properties->GetText());

	return nullptr;
}

std::unique_ptr<Shader> Shader::CreateShader(
	GLenum type, const std::string & source)
{
	auto shader = std::make_unique<Shader>(glCreateShader(type));

	if( shader->Init(source) )
		return std::move(shader);

	return nullptr;
}

ShaderProgram::ShaderProgram()
	:  program_handle_{glCreateProgram()}
{}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(program_handle_);
}

bool ShaderProgram::AttachShader(
	AssetManager* asset_manager, AssetHandle handle)
{
	auto shader_ptr = asset_manager->GetAsset<Shader>(handle);
	if( !shader_ptr )
		return false;

	glAttachShader(program_handle_, shader_ptr->GetHandle());
	shader_handles_.push_back(handle);
	return true;
}

void ShaderProgram::BindAttribLocation(const std::string& name, unsigned int index)
{
	glBindAttribLocation(program_handle_, index, name.c_str());
}

void ShaderProgram::BindFragDataLocation(const std::string& name, unsigned int index)
{
	glBindFragDataLocation(program_handle_, index, name.c_str());
}

bool ShaderProgram::Link()
{
	if( shader_handles_.empty() )
		return false;

	glLinkProgram(program_handle_);

	int status;
	glGetProgramiv(
		program_handle_,
		GL_LINK_STATUS,
		&status
	);

	if( status == int(GL_FALSE) )
	{
		GLint max_length = 0;
		glGetProgramiv(program_handle_, GL_INFO_LOG_LENGTH, &max_length);

		std::string error_message;
		error_message.resize(max_length);
		GLsizei size;
		glGetProgramInfoLog(
			program_handle_, max_length, &max_length, &error_message[0]);

		BEMBEL_LOG_ERROR()
			<< "Can't link ShaderProgram" << std::endl
			<< error_message << std::endl;

		ready_to_use_ = false;
		return false;
	}

	ready_to_use_ = true;
	return true;
}

bool ShaderProgram::Use()
{
	if( !ready_to_use_ )
		return false;

	glUseProgram(program_handle_);
	return true;
}

const std::string& ShaderProgram::GetTypeName()
{
	const static std::string type_name = "ShaderProgram";
	return type_name;
}

std::unique_ptr<ShaderProgram> ShaderProgram::LoadAsset(
	AssetManager * asset_manager, const std::string& file_name)
{
	xml::Document doc;
	if( doc.LoadFile(file_name.c_str()) != tinyxml2::XML_SUCCESS )
	{
		BEMBEL_LOG_ERROR()
			<< "Failed to load file '" << file_name << "'\n"
			<< doc.ErrorName() << std::endl;
		return nullptr;
	}

	const xml::Element* root = doc.FirstChildElement(GetTypeName().c_str());
	if( !root )
	{
		BEMBEL_LOG_ERROR()
			<< "File '" << file_name << "' has no root element 'Shader'"
			<< std::endl;
		return nullptr;
	}
	auto pragram = CreateAsset(asset_manager, root);
	if( !pragram )
	{
		BEMBEL_LOG_ERROR()
			<< "Failed to create shader program from file '" << file_name << "'"
			<< std::endl;
	}
	return std::move(pragram);
}

std::unique_ptr<ShaderProgram> ShaderProgram::CreateAsset(
	AssetManager * asset_manager, const xml::Element* properties)
{
	auto programm = std::make_unique<ShaderProgram>();

	for(auto it : xml::IterateChildElements(properties, "Shader") )
	{
		auto shader = asset_manager->RequestAsset<Shader>(it);

		if( !programm->AttachShader(asset_manager, shader) )
			return nullptr;
	}

	if( !programm->Link() )
	{
		return nullptr;
	}

	return std::move(programm);
}

void ShaderProgram::DeleteAsset(
	AssetManager* asset_manager, 
	std::unique_ptr<ShaderProgram> program)
{
	std::vector<AssetHandle> shaders = program->shader_handles_;
	program.release();

	for( auto shader_handle : shaders )
		asset_manager->ReleaseAsset(shader_handle);
}

GLint ShaderProgram::GetUniformLocation(const std::string& name) const
{
	auto it = uniorm_locations_.find(name);
	if( it != uniorm_locations_.end() )
		return it->second;

	GLint location = glGetUniformLocation(program_handle_, name.c_str());

	if( location >= 0 )
	{
		// add location to the map for faster lookup in the future.
		uniorm_locations_.emplace(name, location);
	}

	return location;
}

GLuint ShaderProgram::GetUniformBlockIndex(const std::string& name) const
{
	return  glGetUniformBlockIndex(program_handle_, name.c_str());
}

GLint ShaderProgram::GetUniformBlockDataSize(GLuint block_index) const
{
	GLint size;
	glGetActiveUniformBlockiv(
		program_handle_, block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
	return size;
}
GLint ShaderProgram::GetUniformBlockActiveUniforms(GLuint block_index) const
{
	GLint active_uniforms;
	glGetActiveUniformBlockiv(
		program_handle_, block_index, 
		GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &active_uniforms);
	return active_uniforms;
}

void ShaderProgram::GetUniformBlockActiveUniformIndices(
	GLuint block_index, std::vector<GLint>* indices) const
{
	indices->resize(GetUniformBlockActiveUniforms(block_index));
	GLint active_uniforms;
	glGetActiveUniformBlockiv(
		program_handle_, block_index,
		GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &(*indices)[0]);

}

GLint ShaderProgram::GetActiveUniformOffset(GLuint uniform_index) const
{
	GLint offset;
	glGetActiveUniformsiv(
		program_handle_, 1,
		&uniform_index, GL_UNIFORM_OFFSET, &offset);
	return offset;
}

void ShaderProgram::GetActiveUniform(
	GLuint uniform_index, GLint* size, GLenum* type, std::string* name) const
{
	GLint name_length;

	glGetActiveUniformsiv(
		program_handle_, 1,
		&uniform_index, GL_UNIFORM_NAME_LENGTH, &name_length);
	GLchar* buffer = new GLchar[name_length];
	glGetActiveUniform(
		program_handle_, uniform_index, name_length, nullptr, size, type, buffer);

	*name = buffer;
	delete[] buffer;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
