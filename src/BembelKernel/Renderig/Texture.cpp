/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Texture.h"

#include <BembelBase/Logging/Logger.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

Texture::Texture(GLenum target, GLenum format )
	: _target(target)
	, _format(format)
	, _handle(0)
{
}

Texture::~Texture()
{
	Cleanup();
}

void Texture::Init(
	GLenum minFilter,
	GLenum magFilter,
	GLenum warpS,
	GLenum warpT)
{
	glGenTextures(1, &_handle);
	glBindTexture(_target, _handle);
	glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minFilter));
	glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magFilter));
	glTexParameteri(_target, GL_TEXTURE_WRAP_S, static_cast<GLint>(warpS));
	glTexParameteri(_target, GL_TEXTURE_WRAP_T, static_cast<GLint>(warpT));
	glBindTexture(_target, 0);
}
void Texture::Init(
	const glm::uvec2& size,
	GLenum minFilter,
	GLenum magFilter,
	GLenum warpS,
	GLenum warpT)
{
	glGenTextures(1, &_handle);
	glBindTexture(_target, _handle);
	glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minFilter));
	glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magFilter));
	glTexParameteri(_target, GL_TEXTURE_WRAP_S, static_cast<GLint>(warpS));
	glTexParameteri(_target, GL_TEXTURE_WRAP_T, static_cast<GLint>(warpT));
	
	if (_format == GL_DEPTH_COMPONENT ||
		_format == GL_DEPTH_COMPONENT16||
		_format == GL_DEPTH_COMPONENT24||
		_format == GL_DEPTH_COMPONENT32)
	{
		glTexImage2D(
			_target, 0, static_cast<GLint>(_format),
			size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}
	else
	{
		glTexImage2D(
			_target, 0, static_cast<GLint>(_format),
			size.x, size.y, 0, GL_RGBA, GL_FLOAT, nullptr);
	}
	glBindTexture(_target, 0);
}
void Texture::Init(
	const Image& image,
	GLenum minFilter, 
	GLenum magFilter,
	GLenum warpS,
	GLenum warpT)
{
	glGenTextures(1, &_handle);
	glBindTexture(_target, _handle);
	glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minFilter));
	glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magFilter));
	glTexParameteri(_target, GL_TEXTURE_WRAP_S, static_cast<GLint>(warpS));
	glTexParameteri(_target, GL_TEXTURE_WRAP_T, static_cast<GLint>(warpT));
	glTexImage2D(
		_target, 0, static_cast<GLint>(_format),
		image.GetWidth(), image.GetHeight(), 0,
		image.GetChannels() == 4 ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE, image.GetData()
		);
	glBindTexture(_target, 0);
}

void Texture::Cleanup()
{
	if(_handle != 0)
		glDeleteTextures(1, &_handle);
	_handle = 0;
}
void Texture::Bind() const
{
	glBindTexture(_target, _handle);
}
void Texture::Release() const
{
	glBindTexture(_target, 0);
}

bool Texture::SetSize(const glm::ivec2& size)
{
	if (_handle == 0)
		return false;

	glBindTexture(_target, _handle);
	glTexImage2D(
		_target, 0, static_cast<GLint>(_format),
		size.x, size.y, 0,
		GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(_target, 0);
	return true;
}

bool Texture::SetData(const Image& image, GLint mipMapLevel)
{
	if (_handle == 0)
		return false;

	glBindTexture(_target, _handle);
	glTexImage2D(
		_target, mipMapLevel, static_cast<GLint>(_format),
		image.GetWidth(), image.GetHeight(), 0,
		image.GetChannels() == 4 ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE, image.GetData());
	glBindTexture(_target, 0);
	return true;
}

GLuint Texture::GetTextureHandle() const
{
	return _handle;
}

GLenum Texture::GetTextureFormat() const
{
	return _format;
}

GLenum Texture::GetTextureTarget() const
{
	return _target;
}

const std::string& Texture::GetTypeName()
{
	const static std::string typeName = "Texture";
	return typeName;
}

// Texture* Texture::LoadeAsset(const AssetDescription& asset, AssetManager*)
// {
// 	std::string file;
// 	if (!asset.GetProperty("file", file))
// 		return nullptr;
// 
// 	Image image;
// 	if (!image.Load(file))
// 		return nullptr;
// 
// 	Texture* texture = new Texture(GL_TEXTURE_2D, GL_RGBA);
// 	texture->Init(image);
// 	return texture;
// }

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/