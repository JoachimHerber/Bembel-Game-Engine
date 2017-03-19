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

void KeyCallback(GLFWwindow* glfw, int keyID, int scancode, int action, int mods)
{
	auto window = GetWindow(glfw);
	auto kernel = window->GetDisplayManager()->GetKernel();
	auto event_manager = kernel->GetEventManager();

	if( action == GLFW_PRESS )
		event_manager->Broadcast(KeyPressEvent{window, keyID, scancode, mods});
	else if( action == GLFW_REPEAT )
		event_manager->Broadcast(KeyRepeatEvent{window, keyID, scancode, mods});
	else if( action == GLFW_RELEASE )
		event_manager->Broadcast(KeyReleaseEvent{window, keyID, scancode, mods});
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

void MouseButtonCallback(GLFWwindow* glfw, int buttonID, int action, int mods)
{
	auto window = GetWindow(glfw);
	auto kernel = window->GetDisplayManager()->GetKernel();
	auto event_manager = kernel->GetEventManager();

	if( action == GLFW_PRESS )
		event_manager->Broadcast(MouseButtonPressEvent{window, buttonID, mods});
	else if( action == GLFW_REPEAT )
		event_manager->Broadcast(MouseButtonRepeatEvent{window, buttonID, mods});
	else if( action == GLFW_RELEASE )
		event_manager->Broadcast(MouseButtonReleaseEvent{window, buttonID, mods});
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
