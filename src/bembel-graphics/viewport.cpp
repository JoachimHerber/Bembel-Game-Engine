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
	: _view(view)
{}

Viewport::Viewport(
	std::shared_ptr<View> view, 
	const glm::ivec2& pos, 
	const glm::ivec2& size)
	: _view(view)
	, _position(pos)
	, _size(size)
{}

Viewport::~Viewport()
{}

const glm::ivec2& Viewport::GetPosition() const
{
	return _position;
}
const glm::ivec2& Viewport::GetSize() const
{
	return _size;
}

void Viewport::Enable()
{
	_enabled = true;
}
void Viewport::Disable()
{
	_enabled = false;
}
bool Viewport::IsEnabled()
{
	return _enabled;
}

void Viewport::SetView(std::shared_ptr<View> view)
{
	_view = view;
}

std::shared_ptr<Viewport::View> Viewport::GetView() const
{
	return _view;
}

void Viewport::UpdatePosition(const glm::vec2& frameBufferSize)
{
	_position = glm::ivec2(frameBufferSize*_relativPosition) + _positionOffset;
}
void Viewport::UpdateSize(const glm::vec2& frameBufferSize)
{
	_size = glm::ivec2(frameBufferSize*_relativSize) + _sizeOffset;
}

const glm::vec2& Viewport::GetRelativPosition() const
{
	return _relativPosition;
}

void Viewport::SetRelativPosition(const glm::vec2& val)
{
	_relativPosition = val;
}

const glm::vec2& Viewport::GetRelativSize() const
{
	return _relativSize;
}

void Viewport::SetRelativSize(const glm::vec2& val)
{
	_relativSize = val;
}

const glm::vec2& Viewport::GetPositionOffset() const
{
	return _positionOffset;
}

void Viewport::SetPositionOffset(const glm::vec2& val)
{
	_positionOffset = val;
}

const glm::vec2& Viewport::GetSizeOffset() const
{
	return _sizeOffset;
}

void Viewport::SetSizeOffset(const glm::vec2& val)
{
	_sizeOffset = val;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
