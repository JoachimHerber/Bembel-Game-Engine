/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "frame-buffer-object.h"
#include "texture.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

FrameBufferObject::FrameBufferObject()
	: handle_(0)
	, depth_attechment_{nullptr, 0}
{}

FrameBufferObject::~FrameBufferObject()
{
	CleanUp();
}

void FrameBufferObject::Init()
{
	glGenFramebuffers(1, &handle_);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle_);
	if( depth_attechment_.texture )
	{
		glFramebufferTexture2D(
			GL_DRAW_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT,
			depth_attechment_.texture->GetTextureTarget(),
			depth_attechment_.texture->GetTextureHandle(),
			depth_attechment_.level);
	}
	for( size_t n = 0; n<color_attechments_.size(); ++n )
	{
		if( color_attechments_[n].texture )
		{
			glFramebufferTexture2D(
				GL_DRAW_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0 + n,
				color_attechments_[n].texture->GetTextureTarget(),
				color_attechments_[n].texture->GetTextureHandle(),
				color_attechments_[n].level);
		}
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void FrameBufferObject::CleanUp()
{
	if( handle_ != 0 )
	{
		glDeleteFramebuffers(1, &handle_);
		handle_ = 0;
	}
}

void FrameBufferObject::RemoveAllAttechments()
{
	SetDepthAttechment(nullptr);
	for( size_t n = 0; n < color_attechments_.size(); ++n )
		SetColorAttechment(n, nullptr);
}

void FrameBufferObject::SetDepthAttechment(Texture* texture, GLint level /*= 0*/)
{
	depth_attechment_.texture = texture;
	depth_attechment_.level = level;

	if( handle_ != 0 )
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle_);
		glFramebufferTexture2D(
			GL_DRAW_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT,
			texture->GetTextureTarget(),
			texture->GetTextureHandle(),
			level);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle_);
	}
}

void FrameBufferObject::SetColorAttechment(unsigned index, Texture* texture, GLint level /*= 0*/)
{
	while( color_attechments_.size() <= index )
		color_attechments_.push_back(Attechment{nullptr, 0});

	color_attechments_[index].texture = texture;
	color_attechments_[index].level = level;

	if( handle_ != 0 )
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle_);
		glFramebufferTexture2D(
			GL_DRAW_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0 + index,
			texture->GetTextureTarget(),
			texture->GetTextureHandle(),
			level);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle_);
	}
}

void FrameBufferObject::BeginRenderToTexture()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle_);

	GLenum status;
	status = glCheckFramebufferStatusEXT(GL_DRAW_FRAMEBUFFER);

	std::vector<GLenum> draw_buffers;
	for( size_t n = 0; n < color_attechments_.size(); ++n )
	{
		if( color_attechments_[n].texture )
			draw_buffers.push_back(GL_COLOR_ATTACHMENT0 + n);
	}

	glDrawBuffers(draw_buffers.size(), &draw_buffers[0]);
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
