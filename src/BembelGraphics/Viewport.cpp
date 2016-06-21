/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Viewport.h"

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
void Viewport::SetPosition(const glm::ivec2& pos)
{
	_position = pos;
}

const glm::ivec2& Viewport::GetSize() const
{
	return _size;
}
void Viewport::SetSize(const glm::ivec2& size)
{
	_size = size;
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

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
