/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "shader.h"

#include <fstream>

#include <bembel-base/logging/logger.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

Shader::Shader()
	: _programHandle(glCreateProgram())
{}

Shader::~Shader()
{
	glDeleteProgram(_programHandle);
	for (auto it : _shaderHandles)
		glDeleteShader(it);
}

bool Shader::AttachShader(GLenum type, const std::string& source)
{
	GLint shaderHandle = glCreateShader(type);

	const char* c_str = source.c_str();
	const int   length = static_cast<int>(source.length());

	glShaderSource(shaderHandle, 1, &c_str, &length);
	glCompileShader(shaderHandle);

	int status;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);
	if (status != int(GL_TRUE))
	{
		char errorMessage[4096];
		GLsizei size;
		glGetShaderInfoLog(
			shaderHandle, 4096, &size, errorMessage);

		BEMBEL_LOG_ERROR()
			<< "Can't compile Shader" << std::endl
			<< errorMessage << std::endl;

		glDeleteShader(shaderHandle);
		return false;
	}

	glAttachShader(_programHandle, shaderHandle);
	_shaderHandles.push_back(shaderHandle);
	return true;
}

bool Shader::AttachShaderFromFile(GLenum type, const std::string& fileName)
{
	std::fstream file(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!file.is_open())
	{
		BEMBEL_LOG_ERROR()
			<< "unable to find shader file "
			<< "'" << fileName << "'"
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
	glBindAttribLocation(_programHandle, index, name.c_str());
}

void Shader::BindFragDataLocation(const std::string& name, unsigned int index)
{
	glBindFragDataLocation(_programHandle, index, name.c_str());
}

bool Shader::Link()
{
	if (_shaderHandles.empty())
		return false;

	glLinkProgram(_programHandle);

	int status;
	glGetProgramiv(
		_programHandle,
		GL_LINK_STATUS,
		&status
		);

	if (status == int(GL_FALSE))
	{
		GLint maxLength = 0;
		glGetProgramiv(_programHandle, GL_INFO_LOG_LENGTH, &maxLength);

		std::string errorMessage;
		errorMessage.resize(maxLength);
		GLsizei size;
		glGetProgramInfoLog(_programHandle, maxLength, &maxLength, &errorMessage[0]);

		BEMBEL_LOG_ERROR()
			<< "Can't link ShaderProgram" << std::endl
			<< errorMessage << std::endl;

		_readyToUse = false;
		return false;
	}

	_readyToUse = true;
	return true;
}

bool Shader::Use()
{
	if (!_readyToUse)
		return false;
	
	glUseProgram(_programHandle);
	return true;
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
	auto it = _uniormLocations.find(name);
	if (it != _uniormLocations.end())
		return it->second;

	GLint location = glGetUniformLocation(_programHandle, name.c_str());

	if (location >= 0)
	{
		// add location to the map for faster lookup in the future.
		_uniormLocations.emplace(name, location);
	}

	return location;
}

GLuint Shader::GetUniformBlockIndex(const std::string& name) const
{
	return  glGetUniformBlockIndex(_programHandle, name.c_str());
}

GLint Shader::GetUniformBlockDataSize( GLuint index ) const
{
	GLint size;
	glGetActiveUniformBlockiv( 
		_programHandle, index, GL_UNIFORM_BLOCK_DATA_SIZE, &size );
	return size;
}

} //end of namespace bembel
  /*============================================================================*/
  /* END OF FILE                                                                */
  /*============================================================================*/
