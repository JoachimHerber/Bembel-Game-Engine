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

#include "Texture.h"

#include <bembel-base/logging/logger.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

Texture::Texture(GLenum target, GLenum format)
	: target_(target)
	, format_(format)
	, handle_(0)
{}

Texture::~Texture()
{
	Cleanup();
}

void Texture::Init(
	GLenum min_filter,
	GLenum mag_filter,
	GLenum warp_s,
	GLenum warp_t)
{
	glGenTextures(1, &handle_);
	glBindTexture(target_, handle_);
	glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min_filter));
	glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag_filter));
	glTexParameteri(target_, GL_TEXTURE_WRAP_S, static_cast<GLint>(warp_s));
	glTexParameteri(target_, GL_TEXTURE_WRAP_T, static_cast<GLint>(warp_t));
	glBindTexture(target_, 0);
}
void Texture::Init(
	const glm::uvec2& size,
	GLenum min_filter,
	GLenum mag_filter,
	GLenum warp_s,
	GLenum warp_t)
{
	glGenTextures(1, &handle_);
	glBindTexture(target_, handle_);
	glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min_filter));
	glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag_filter));
	glTexParameteri(target_, GL_TEXTURE_WRAP_S, static_cast<GLint>(warp_s));
	glTexParameteri(target_, GL_TEXTURE_WRAP_T, static_cast<GLint>(warp_t));

	if( format_ == GL_DEPTH_COMPONENT ||
		format_ == GL_DEPTH_COMPONENT16||
		format_ == GL_DEPTH_COMPONENT24||
		format_ == GL_DEPTH_COMPONENT32 )
	{
		glTexImage2D(
			target_, 0, static_cast<GLint>(format_),
			size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}
	else
	{
		glTexImage2D(
			target_, 0, static_cast<GLint>(format_),
			size.x, size.y, 0, GL_RGBA, GL_FLOAT, nullptr);
	}
	glBindTexture(target_, 0);
}
void Texture::Init(
	const Image& image,
	GLenum min_filter,
	GLenum mag_filter,
	GLenum warp_s,
	GLenum warp_t)
{
	glGenTextures(1, &handle_);
	glBindTexture(target_, handle_);
	glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min_filter));
	glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag_filter));
	glTexParameteri(target_, GL_TEXTURE_WRAP_S, static_cast<GLint>(warp_s));
	glTexParameteri(target_, GL_TEXTURE_WRAP_T, static_cast<GLint>(warp_t));
	glTexImage2D(
		target_, 0, static_cast<GLint>(format_),
		image.GetWidth(), image.GetHeight(), 0,
		image.GetChannels() == 4 ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE, image.GetData()
	);
	glBindTexture(target_, 0);
}

void Texture::Cleanup()
{
	if( handle_ != 0 )
		glDeleteTextures(1, &handle_);
	handle_ = 0;
}
void Texture::Bind() const
{
	glBindTexture(target_, handle_);
}
void Texture::Release() const
{
	glBindTexture(target_, 0);
}

bool Texture::SetSize(const glm::ivec2& size)
{
	if( handle_ == 0 )
		return false;

	glBindTexture(target_, handle_);
	glTexImage2D(
		target_, 0, static_cast<GLint>(format_),
		size.x, size.y, 0,
		GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(target_, 0);
	return true;
}

bool Texture::SetData(const Image& image, GLint mipMapLevel)
{
	if( handle_ == 0 )
		return false;

	glBindTexture(target_, handle_);
	glTexImage2D(
		target_, mipMapLevel, static_cast<GLint>(format_),
		image.GetWidth(), image.GetHeight(), 0,
		image.GetChannels() == 4 ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE, image.GetData());
	glBindTexture(target_, 0);
	return true;
}

GLuint Texture::GetTextureHandle() const
{
	return handle_;
}

GLenum Texture::GetTextureFormat() const
{
	return format_;
}

GLenum Texture::GetTextureTarget() const
{
	return target_;
}

const std::string& Texture::GetTypeName()
{
	const static std::string type_name = "Texture";
	return type_name;
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