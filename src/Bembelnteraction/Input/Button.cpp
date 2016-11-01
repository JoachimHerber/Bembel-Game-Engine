/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Button.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

Button::Button(const std::string& name)
	: _name(name)
	, _pressed(false)
{}

Button::~Button()
{}

const std::string& Button::GetName()const
{
	return _name;
}

bool Button::GetIsPressed() const
{
	return _pressed;
}

void Button::SetIsPressed(bool b)
{
	if (_pressed == b)
		return;

	_pressed = b;
	if (_pressed)
		_pressSignal.Emit();
	else
		_releaseSignal.Emit();
}

Signal<>& Button::GetPressSignal()
{
	return _pressSignal;
}

Signal<>& Button::GetReleaseSignal()
{
	return _releaseSignal;
}

BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(ButtonPressEvent);
BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(ButtonReleaseEvent);

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
