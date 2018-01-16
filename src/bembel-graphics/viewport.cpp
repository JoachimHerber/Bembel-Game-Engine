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

#include "viewport.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

Viewport::Viewport()
{}
Viewport::Viewport(std::shared_ptr<View> view)
	: view_(view)
{}

Viewport::~Viewport()
{}

const glm::ivec2& Viewport::GetPosition() const
{
	return position_;
}
const glm::uvec2& Viewport::GetSize() const
{
	return size_;
}

void Viewport::Enable()
{
	enabled_ = true;
}
void Viewport::Disable()
{
	enabled_ = false;
}
bool Viewport::IsEnabled()
{
	return enabled_;
}

void Viewport::SetView(std::shared_ptr<View> view)
{
	view_ = view;
}

std::shared_ptr<Viewport::View> Viewport::GetView() const
{
	return view_;
}

void Viewport::UpdatePosition(const glm::vec2& frame_buffer_size)
{
	position_ = glm::ivec2(
		frame_buffer_size*relativ_position_ + position_offset_);
}
void Viewport::UpdateSize(const glm::vec2& frame_buffer_size)
{
	size_ = glm::ivec2(
		frame_buffer_size*relativ_size_ + size_offset_);
}

const glm::vec2& Viewport::GetRelativPosition() const
{
	return relativ_position_;
}

void Viewport::SetRelativPosition(const glm::vec2& val)
{
	relativ_position_ = val;
}

const glm::vec2& Viewport::GetRelativSize() const
{
	return relativ_size_;
}

void Viewport::SetRelativSize(const glm::vec2& val)
{
	relativ_size_ = val;
}

const glm::vec2& Viewport::GetPositionOffset() const
{
	return position_offset_;
}

void Viewport::SetPositionOffset(const glm::vec2& val)
{
	position_offset_ = val;
}

const glm::vec2& Viewport::GetSizeOffset() const
{
	return size_offset_;
}

void Viewport::SetSizeOffset(const glm::vec2& val)
{
	size_offset_ = val;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
