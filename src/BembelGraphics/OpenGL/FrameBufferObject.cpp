/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "FrameBufferObject.h"
#include "Texture.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

FrameBufferObject::FrameBufferObject()
	: _handle(0)
{}

FrameBufferObject::~FrameBufferObject()
{
	CleanUp();
}

void FrameBufferObject::Init()
{
	glGenFramebuffers(1, &_handle);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _handle);
	if (_depthAttechment.texture)
	{
		glFramebufferTexture2D(
			GL_DRAW_FRAMEBUFFER, 
			GL_DEPTH_ATTACHMENT,
			_depthAttechment.texture->GetTextureTarget(),
			_depthAttechment.texture->GetTextureHandle(),
			_depthAttechment.level);
	}
	for (size_t n = 0; n<_colorAttechments.size(); ++n)
	{
		if (_colorAttechments[n].texture)
		{
			glFramebufferTexture2D(
				GL_DRAW_FRAMEBUFFER, 
				GL_COLOR_ATTACHMENT0 + n,
				_colorAttechments[n].texture->GetTextureTarget(),
				_colorAttechments[n].texture->GetTextureHandle(),
				_colorAttechments[n].level);
		}
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void FrameBufferObject::CleanUp()
{
	if (_handle != 0)
	{
		glDeleteFramebuffers(1, &_handle);
		_handle = 0;
	}
}

void FrameBufferObject::RemoveAllAttechments()
{
	SetDepthAttechment(nullptr);
	for (size_t n = 0; n < _colorAttechments.size(); ++n)
		SetColorAttechment(n, nullptr);
}

void FrameBufferObject::SetDepthAttechment(TexturePtr texture, GLint level /*= 0*/)
{
	_depthAttechment.texture = texture;
	_depthAttechment.level = level;

	if (_handle != 0)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _handle);
		glFramebufferTexture2D(
			GL_DRAW_FRAMEBUFFER, 
			GL_DEPTH_ATTACHMENT,
			texture->GetTextureTarget(),
			texture->GetTextureHandle(),
			level);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _handle);
	}
}

void FrameBufferObject::SetColorAttechment(unsigned index, TexturePtr texture, GLint level /*= 0*/)
{
	while (_colorAttechments.size() <= index)
		_colorAttechments.push_back(Attechment{nullptr, 0});

	_colorAttechments[index].texture = texture;
	_colorAttechments[index].level   = level;

	if (_handle != 0)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _handle);
		glFramebufferTexture2D(
			GL_DRAW_FRAMEBUFFER, 
			GL_COLOR_ATTACHMENT0 + index,
			texture->GetTextureTarget(),
			texture->GetTextureHandle(),
			level);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _handle);
	}
}

void FrameBufferObject::BeginRenderToTexture()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _handle);

	GLenum status;
	status = glCheckFramebufferStatusEXT(GL_DRAW_FRAMEBUFFER);

	std::vector<GLenum> drawBuffers;
	for (size_t n = 0; n < _colorAttechments.size(); ++n)
	{
		if (_colorAttechments[n].texture)
			drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + n);
	}

	glDrawBuffers(drawBuffers.size(), &drawBuffers[0]);
}

void FrameBufferObject::EndRenderToTexture()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
