/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "texture-view.h"

#include <string>

#include <bembel-kernel/rendering/frame-buffer-object.h>
#include <bembel-kernel/rendering/texture.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

TextureView::TextureView(AssetManager* asset_manager, Texture* texture)
	: asset_manager_{asset_manager}
	, texture_(texture)
	, view_area_pos_(0, 0)
	, view_area_size_(1, 1)
	, fbo_(std::make_unique<FrameBufferObject>())
{
	fbo_->SetColorAttechment(0, texture);
}

TextureView::~TextureView()
{}

void TextureView::SetViewArea(
	const glm::ivec2& pos, const glm::uvec2& size)
{
	view_area_pos_ = pos;
	view_area_size_ = size;
}

glm::ivec2 TextureView::GetViewAreaPosition() const
{
	return view_area_pos_;
}

glm::uvec2 TextureView::GetViewAreaSize() const
{
	return view_area_size_;
}

void TextureView::Init()
{
	fbo_->Init();
}

void TextureView::Cleanup()
{
	fbo_->CleanUp();
}

void TextureView::Draw(
	const glm::ivec2& viewport_position,
	const glm::uvec2& viewport_size)
{
	fbo_->BlitToBackBuffer(
		view_area_pos_,
		view_area_pos_ + glm::ivec2(view_area_size_),
		viewport_position,
		viewport_position + glm::ivec2(viewport_size)
	);
	/*/
	shader_->Use();
	glUniform4f(uniform_, min_.x, min_.y, max_.x-min_.x, max_.y-min_.y);

	texture_->Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	texture_->Release();

	glUseProgram(0);
	//*/
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
