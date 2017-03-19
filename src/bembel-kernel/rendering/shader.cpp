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

Shader::Shader()
	: program_handle_(glCreateProgram())
{}

Shader::~Shader()
{
	glDeleteProgram(program_handle_);
	for( auto it : shader_handles_ )
		glDeleteShader(it);
}

bool Shader::AttachShader(GLenum type, const std::string& source)
{
	GLint shader_handle = glCreateShader(type);

	const char* c_str = source.c_str();
	const int   length = static_cast<int>(source.length());

	glShaderSource(shader_handle, 1, &c_str, &length);
	glCompileShader(shader_handle);

	int status;
	glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &status);
	if( status != int(GL_TRUE) )
	{
		char error_message[4096];
		GLsizei size;
		glGetShaderInfoLog(
			shader_handle, 4096, &size, error_message);

		BEMBEL_LOG_ERROR()
			<< "Can't compile Shader" << std::endl
			<< error_message << std::endl;

		glDeleteShader(shader_handle);
		return false;
	}

	glAttachShader(program_handle_, shader_handle);
	shader_handles_.push_back(shader_handle);
	return true;
}

bool Shader::AttachShaderFromFile(GLenum type, const std::string& file_name)
{
	std::fstream file(
		file_name.c_str(), std::ios_base::in | std::ios_base::binary);
	if( !file.is_open() )
	{
		BEMBEL_LOG_ERROR()
			<< "unable to find shader file "
			<< "'" << file_name << "'"
			<< std::endl;
		return nullptr;
	}

	// determine file size
	file.seekg(0, file.end);
	std::size_t length = file.tellg();
	file.seekg(0, file.beg);

	// read the program from file
	std::string source;
	source.resize(length +1, '\0');
	file.read(&source[0], length);
	file.close();

	return AttachShader(type, source);
}

void Shader::BindAttribLocation(const std::string& name, unsigned int index)
{
	glBindAttribLocation(program_handle_, index, name.c_str());
}

void Shader::BindFragDataLocation(const std::string& name, unsigned int index)
{
	glBindFragDataLocation(program_handle_, index, name.c_str());
}

bool Shader::Link()
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

bool Shader::Use()
{
	if( !ready_to_use_ )
		return false;

	glUseProgram(program_handle_);
	return true;
}

GLint Shader::GetUniformLocation(const std::string& name) const
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

GLuint Shader::GetUniformBlockIndex(const std::string& name) const
{
	return  glGetUniformBlockIndex(program_handle_, name.c_str());
}

GLint Shader::GetUniformBlockDataSize(GLuint index) const
{
	GLint size;
	glGetActiveUniformBlockiv(
		program_handle_, index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
	return size;
}

} //end of namespace bembel
  /*============================================================================*/
  /* END OF FILE                                                                */
  /*============================================================================*/
