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

#include "event-callbacks.h"
#include "display-events.h"
#include "input-events.h"
#include "../kernel.h"
#include "../display/window.h"
#include "../display/display-manager.h"

#include <GLFW/glfw3.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {
namespace glfw_callbacks {
Window* GetWindow(GLFWwindow* glfw)
{
	return static_cast<Window*>(glfwGetWindowUserPointer(glfw));
}

void WindowPositionCallback(GLFWwindow* glfw, int x, int y)
{
	auto window = GetWindow(glfw);
	auto kernel = window->GetDisplayManager()->GetKernel();
	auto event_manager = kernel->GetEventManager();

	event_manager->Broadcast(WindowMovedEvent{window, glm::vec2(x, y)});
}
void WindowSizeCallback(GLFWwindow* glfw, int w, int h)
{
	auto window = GetWindow(glfw);
	auto kernel = window->GetDisplayManager()->GetKernel();
	auto event_manager = kernel->GetEventManager();

	event_manager->Broadcast(WindowResizeEvent{window, glm::vec2(w, h)});
}
void WindowCloseCallback(GLFWwindow* glfw)
{
	auto window = GetWindow(glfw);
	auto kernel = window->GetDisplayManager()->GetKernel();
	auto event_manager = kernel->GetEventManager();

	event_manager->Broadcast(WindowShouldCloseEvent{window});
}
void WindowRefreshCallback(GLFWwindow* glfw)
{
	auto window = GetWindow(glfw);
	auto kernel = window->GetDisplayManager()->GetKernel();
	auto event_manager = kernel->GetEventManager();
	// currently unused
}
void WindowFocusCallback(GLFWwindow* glfw, int focused)
{
	auto window = GetWindow(glfw);
	auto kernel = window->GetDisplayManager()->GetKernel();
	auto event_manager = kernel->GetEventManager();

	if( focused )
		event_manager->Broadcast(WindowGainedFocusEvent{window});
	else
		event_manager->Broadcast(WindowLostFocusEvent{window});
}
void WindowIconifyCallback(GLFWwindow* glfw, int iconified)
{
	auto window = GetWindow(glfw);
	auto kernel = window->GetDisplayManager()->GetKernel();
	auto event_manager = kernel->GetEventManager();

	if( iconified )
		event_manager->Broadcast(WindowIconifyedEvent{window});
	else
		event_manager->Broadcast(WindowRestoredEvent{window});
}

void FramebufferSizeCallback(GLFWwindow* glfw, int w, int h)
{
	auto window = GetWindow(glfw);
	auto kernel = window->GetDisplayManager()->GetKernel();
	auto event_manager = kernel->GetEventManager();

	event_manager->Broadcast(FrameBufferResizeEvent{window, glm::vec2(w, h)});
}

void KeyCallback(GLFWwindow* glfw, int key_id, int scancode, int action, int mods)
{
	auto window = GetWindow(glfw);
	auto kernel = window->GetDisplayManager()->GetKernel();
	auto event_manager = kernel->GetEventManager();

	if( action == GLFW_PRESS )
		event_manager->Broadcast(KeyPressEvent{window, key_id, scancode, mods});
	else if( action == GLFW_REPEAT )
		event_manager->Broadcast(KeyRepeatEvent{window, key_id, scancode, mods});
	else if( action == GLFW_RELEASE )
		event_manager->Broadcast(KeyReleaseEvent{window, key_id, scancode, mods});
}

void CharCallback(GLFWwindow* glfw, unsigned int c)
{
	auto window = GetWindow(glfw);
	auto kernel = window->GetDisplayManager()->GetKernel();
	auto event_manager = kernel->GetEventManager();

	event_manager->Broadcast(TextInputEvent{window, c});
}
void CharModsCallback(GLFWwindow* glfw, unsigned int c, int mods)
{
	auto window = GetWindow(glfw);
	auto kernel = window->GetDisplayManager()->GetKernel();
	auto event_manager = kernel->GetEventManager();
	// currently unused
}

void MouseButtonCallback(GLFWwindow* glfw, int button_id, int action, int mods)
{
	auto window = GetWindow(glfw);
	auto kernel = window->GetDisplayManager()->GetKernel();
	auto event_manager = kernel->GetEventManager();

	if( action == GLFW_PRESS )
		event_manager->Broadcast(MouseButtonPressEvent{window, button_id, mods});
	else if( action == GLFW_REPEAT )
		event_manager->Broadcast(MouseButtonRepeatEvent{window, button_id, mods});
	else if( action == GLFW_RELEASE )
		event_manager->Broadcast(MouseButtonReleaseEvent{window, button_id, mods});
}

void CursorPositionCallback(GLFWwindow* glfw, double x, double y)
{
	auto window = GetWindow(glfw);
	auto kernel = window->GetDisplayManager()->GetKernel();
	auto event_manager = kernel->GetEventManager();

	event_manager->Broadcast(CursorMovedEvent{window, glm::vec2(x, y)});
}
void CursorEnterCallback(GLFWwindow* glfw, int entered)
{
	auto window = GetWindow(glfw);
	auto kernel = window->GetDisplayManager()->GetKernel();
	auto event_manager = kernel->GetEventManager();

	if( entered )
		event_manager->Broadcast(CursorEnteredEvent{window});
	else
		event_manager->Broadcast(CursorLeftEvent{window});
}

void ScrollCallback(GLFWwindow* glfw, double x, double y)
{
	auto window = GetWindow(glfw);
	auto kernel = window->GetDisplayManager()->GetKernel();
	auto event_manager = kernel->GetEventManager();

	event_manager->Broadcast(ScrollEvent{window, x, y});
}

void DropCallback(GLFWwindow* glfw, int count, const char** files)
{
	auto window = GetWindow(glfw);
	auto kernel = window->GetDisplayManager()->GetKernel();
	auto event_manager = kernel->GetEventManager();

	FileDropEvent event;
	event.window = GetWindow(glfw);
	for( int i = 0; i<count; ++i )
		event.files.push_back(files[i]);
	event_manager->Broadcast(event);
}

void MonitorCallback(GLFWmonitor* glfw, int)
{
	// 	Window*       window = GetWindow(glfw);
	// 	EventManager* eventMgr = window->GetKernel()->GetEventManager();
}

} //end of namespace glfw_callbacks
} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
