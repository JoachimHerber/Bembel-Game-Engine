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
