/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "mouse.h"

#include <GLFW/glfw3.h>

#include <bembel-base/logging/logger.h>
#include <bembel-kernel/events/event-manager.h>

/*============================================================================*/
/* IMPLEMENTATION			                                                  */
/*============================================================================*/
namespace bembel{

Mouse::Mouse(EventManager* eventMgr)
	: InputDevice(eventMgr, "Mouse")
{
	_buttons.push_back(std::make_shared<Button>("Mouse L"));
	_buttons.push_back(std::make_shared<Button>("Mouse R"));
	_buttons.push_back(std::make_shared<Button>("Mouse M"));
	_buttons.push_back(std::make_shared<Button>("Mouse 4"));
	_buttons.push_back(std::make_shared<Button>("Mouse 5"));
	_buttons.push_back(std::make_shared<Button>("Mouse 6"));
	_buttons.push_back(std::make_shared<Button>("Mouse 7"));
	_buttons.push_back(std::make_shared<Button>("Mouse 8"));
	_buttons.push_back(std::make_shared<Button>("Mouse 9"));
	_buttons.push_back(std::make_shared<Button>("Mouse 10"));
	_buttons.push_back(std::make_shared<Button>("Mouse 11"));
	_buttons.push_back(std::make_shared<Button>("Mouse 12"));
	_buttons.push_back(std::make_shared<Button>("Mouse 13"));
	_buttons.push_back(std::make_shared<Button>("Mouse 14"));
	_buttons.push_back(std::make_shared<Button>("Mouse 15"));
	_buttons.push_back(std::make_shared<Button>("Mouse 16"));

	_eventMgr->AddHandler<MouseButtonPressEvent>(this);
	_eventMgr->AddHandler<MouseButtonReleaseEvent>(this);
}

Mouse::~Mouse()
{
	_eventMgr->RemoveHandler<MouseButtonPressEvent>(this);
	_eventMgr->RemoveHandler<MouseButtonReleaseEvent>(this);
}

Button* Mouse::GetButton(int button_id)
{
	if (button_id < 0 || static_cast<size_t>(button_id) >= _buttons.size())
	{
		BEMBEL_LOG_ERROR()
			<< "Mouse button " << button_id << " not supported";
		return nullptr;
	}

	return _buttons[button_id].get();
}

void Mouse::HandleEvent(const MouseButtonPressEvent& event)
{
	auto button = GetButton(event.button_id);
	_eventMgr->Broadcast(ButtonPressEvent{button});
	button->SetIsPressed(true);
}

void Mouse::HandleEvent(const MouseButtonReleaseEvent& event)
{
	auto button = GetButton(event.button_id);
	_eventMgr->Broadcast(ButtonReleaseEvent{button});
	button->SetIsPressed(false);
}

}//end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
