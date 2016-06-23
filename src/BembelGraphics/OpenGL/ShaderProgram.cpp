/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "ShaderProgram.h"

#include <BembelBase/Logging/Logger.h>

#include <fstream>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

ShaderProgram::ShaderProgram()
	: _programHandle(glCreateProgram())
{}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(_programHandle);
	for (auto it : _shaderHandles)
		glDeleteShader(it);
}

bool ShaderProgram::AttachShader(GLenum type, const std::string& source)
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

bool ShaderProgram::AttachShaderFromFile(GLenum type, const std::string& fileName)
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

void ShaderProgram::BindAttribLocation(const std::string& name, unsigned int index)
{
	glBindAttribLocation(_programHandle, index, name.c_str());
}

void ShaderProgram::BindFragDataLocation(const std::string& name, unsigned int index)
{
	glBindFragDataLocation(_programHandle, index, name.c_str());
}

bool ShaderProgram::Link()
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

	if (status != int(GL_TRUE))
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

bool ShaderProgram::Use()
{
	if (!_readyToUse)
		return false;
	
	glUseProgram(_programHandle);
	return true;
}

GLint ShaderProgram::GetUniformLocation(const std::string& name) const
{
	return glGetUniformLocation(_programHandle, name.c_str());
}

GLuint ShaderProgram::GetUniformBlockIndex(const std::string& name) const
{
	return  glGetUniformBlockIndex(_programHandle, name.c_str());
}

} //end of namespace bembel
  /*============================================================================*/
  /* END OF FILE                                                                */
  /*============================================================================*/
