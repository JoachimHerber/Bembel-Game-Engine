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
