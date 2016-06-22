/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "EventCallbacks.h"
#include "Events.h"
#include "../Kernel.h"
#include "../Display/Window.h"
#include "../Display/DisplayManager.h"

#include <GLFW/glfw3.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{
namespace glfw_callbacks{
Window* GetWindow(GLFWwindow* glfw)
{
	return static_cast<Window*>(glfwGetWindowUserPointer(glfw));
}

void WindowPositionCallback(GLFWwindow* glfw, int x, int y)
{
	Window*       window = GetWindow(glfw);
	EventManager* eventMgr = window->GetDisplayManager()->GetKernel()->GetEventManager();

	eventMgr->Broadcast(WindowMovedEvent{window, glm::vec2(x, y)});
}
void WindowSizeCallback(GLFWwindow* glfw, int w, int h)
{
	Window*       window = GetWindow(glfw);
	EventManager* eventMgr = window->GetDisplayManager()->GetKernel()->GetEventManager();

	eventMgr->Broadcast(WindowResizeEvent{window, glm::vec2(w, h)});
}
void WindowCloseCallback(GLFWwindow* glfw)
{
	Window*       window = GetWindow(glfw);
	EventManager* eventMgr = window->GetDisplayManager()->GetKernel()->GetEventManager();

	eventMgr->Broadcast(WindowShouldCloseEvent{window});
}
void WindowRefreshCallback(GLFWwindow* glfw)
{
	Window*       window = GetWindow(glfw);
	EventManager* eventMgr = window->GetDisplayManager()->GetKernel()->GetEventManager();
	// currently unused
}
void WindowFocusCallback(GLFWwindow* glfw, int focused)
{
	Window*       window = GetWindow(glfw);
	EventManager* eventMgr = window->GetDisplayManager()->GetKernel()->GetEventManager();

	if (focused)
		eventMgr->Broadcast(WindowGainedFocusEvent{window});
	else
		eventMgr->Broadcast(WindowLostFocusEvent{window});
}
void WindowIconifyCallback(GLFWwindow* glfw, int iconified)
{
	Window*       window = GetWindow(glfw);
	EventManager* eventMgr = window->GetDisplayManager()->GetKernel()->GetEventManager();

	if (iconified)
		eventMgr->Broadcast(WindowIconifyedEvent{window});
	else
		eventMgr->Broadcast(WindowRestoredEvent{window});
}

void FramebufferSizeCallback(GLFWwindow* glfw, int w, int h)
{
	Window*       window = GetWindow(glfw);
	EventManager* eventMgr = window->GetDisplayManager()->GetKernel()->GetEventManager();

	eventMgr->Broadcast(FrameBufferResizeEvent{window, glm::vec2(w, h)});
}

void KeyCallback(GLFWwindow* glfw, int keyID, int scancode, int action, int mods)
{
	Window*       window = GetWindow(glfw);
	EventManager* eventMgr = window->GetDisplayManager()->GetKernel()->GetEventManager();
	if(action == GLFW_PRESS)
		eventMgr->Broadcast(KeyPressEvent{window, keyID, scancode, mods});
	else if (action == GLFW_REPEAT)
		eventMgr->Broadcast(KeyRepeatEvent{window, keyID, scancode, mods});
	else if (action == GLFW_RELEASE)
		eventMgr->Broadcast(KeyReleaseEvent{window, keyID, scancode, mods});
}

void CharCallback(GLFWwindow* glfw, unsigned int c)
{
	Window*       window = GetWindow(glfw);
	EventManager* eventMgr = window->GetDisplayManager()->GetKernel()->GetEventManager();

	eventMgr->Broadcast(TextInputEvent{window, c});
}
void CharModsCallback(GLFWwindow* glfw, unsigned int c, int mods)
{
	Window*       window = GetWindow(glfw);
	EventManager* eventMgr = window->GetDisplayManager()->GetKernel()->GetEventManager();
	// currently unused
}

void MouseButtonCallback(GLFWwindow* glfw, int buttonID, int action, int mods)
{
	Window*       window = GetWindow(glfw);
	EventManager* eventMgr = window->GetDisplayManager()->GetKernel()->GetEventManager();

	if (action == GLFW_PRESS)
		eventMgr->Broadcast(MouseButtonPressEvent{window, buttonID, mods});
	else if (action == GLFW_REPEAT)
		eventMgr->Broadcast(MouseButtonRepeatEvent{window, buttonID, mods});
	else if (action == GLFW_RELEASE)
		eventMgr->Broadcast(MouseButtonReleaseEvent{window, buttonID, mods});
}

void CursorPositionCallback(GLFWwindow* glfw, double x, double y)
{
	Window*       window = GetWindow(glfw);
	EventManager* eventMgr = window->GetDisplayManager()->GetKernel()->GetEventManager();

	eventMgr->Broadcast(CursorMovedEvent{window, glm::vec2(x, y)});
}
void CursorEnterCallback(GLFWwindow* glfw, int entered)
{
	Window*       window = GetWindow(glfw);
	EventManager* eventMgr = window->GetDisplayManager()->GetKernel()->GetEventManager();

	if (entered)
		eventMgr->Broadcast(CursorEnteredEvent{window});
	else
		eventMgr->Broadcast(CursorLeftEvent{window});
}

void ScrollCallback(GLFWwindow* glfw, double x, double y)
{
	Window*       window = GetWindow(glfw);
	EventManager* eventMgr = window->GetDisplayManager()->GetKernel()->GetEventManager();

	eventMgr->Broadcast(ScrollEvent{window, x, y});
}

void DropCallback(GLFWwindow* glfw, int count, const char** files)
{
	FileDropEvent event;
	event.window = GetWindow(glfw);
	for (int i = 0; i<count; ++i)
		event.files.push_back(files[i]);

	event.window->GetDisplayManager()->GetKernel()->GetEventManager()->Broadcast(event);
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
