/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2017 Joachim Herber                                     * */
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
