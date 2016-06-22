/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "DisplayManager.h"

#include "Window.h"
#include "../Events/Events.h"
#include "../Events/EventCallbacks.h"
#include "../Kernel.h"

#include <BembelBase/Logging/Logger.h>

#include <glfw/glfw3.h>

/*============================================================================*/
/* IMPLEMENTATION			                                                  */
/*============================================================================*/
namespace bembel{

DisplayManager::DisplayManager(Kernel* kernel)
	: _kernel(kernel)
{
	//glfwSetMonitorCallback(glfw_callbacks::MonitorCallback);

	_displayModeFactory.RegisterDefaultObjectGenerator<WindowDisplayMode>("Windowed");
	_displayModeFactory.RegisterDefaultObjectGenerator<FullscreenDisplayMode>("Fullscreen");
}

DisplayManager::~DisplayManager()
{
	DeleteAllWindows();
}

bool DisplayManager::CreateWindows(const xml::Element* properties)
{
	if (!properties)
		return false;

	const xml::Element* windowProperties =
		properties->FirstChildElement("Window");
	
	while (windowProperties)
	{
		_windows.push_back( new Window(this, _windows.size()));

		_windows.back()->Init(windowProperties);

		windowProperties =
			windowProperties->NextSiblingElement("Window");
	}
	return true;
}

void DisplayManager::CloseOpenWindows()
{
	while (!_openWindows.empty())
		_openWindows.front()->Close();

}

void DisplayManager::DeleteAllWindows()
{
	CloseOpenWindows();

	for( Window* window : _windows )
		delete window;

	_windows.clear();
}


void DisplayManager::UpdateWindows()
{
	for (Window* window : _openWindows)
	{
		window->MakeContextCurent();

		glClearColor(0.5f, 0.5f, 0.5f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_kernel->GetEventManager()->Broadcast(WindowUpdateEvent{window});

		window->SwapBuffers();
	}
}

void DisplayManager::OnWindowOpend(Window* window)
{
	_openWindows.push_back(window);
	GLFWwindow* glfw = window->GetGlfwWindow();

	glfwSetWindowUserPointer(glfw, window);

	using namespace glfw_callbacks;
	glfwSetWindowPosCallback(glfw, WindowPositionCallback);
	glfwSetWindowSizeCallback(glfw, WindowSizeCallback);
	glfwSetWindowCloseCallback(glfw, WindowCloseCallback);
	glfwSetWindowRefreshCallback(glfw, WindowRefreshCallback);
	glfwSetWindowFocusCallback(glfw, WindowFocusCallback);
	glfwSetWindowIconifyCallback(glfw, WindowIconifyCallback);
	
	glfwSetFramebufferSizeCallback(glfw, FramebufferSizeCallback);

	glfwSetKeyCallback(glfw, KeyCallback);
	glfwSetCharCallback(glfw, CharCallback);
	glfwSetCharModsCallback(glfw, CharModsCallback);
	
	glfwSetMouseButtonCallback(glfw, MouseButtonCallback);
	glfwSetCursorPosCallback(glfw, CursorPositionCallback);
	glfwSetCursorEnterCallback(glfw, CursorEnterCallback);
	glfwSetScrollCallback(glfw, ScrollCallback);

	glfwSetDropCallback(glfw, DropCallback);

	_kernel->GetEventManager()->Broadcast(WindowOpendEvent{window});
}

void DisplayManager::OnWindowClosed(Window* window)
{
	auto it = std::find(_openWindows.begin(), _openWindows.end(), window);
	if(it != _openWindows.end())
	{
		_openWindows.erase(it);
		_kernel->GetEventManager()->Broadcast(WindowClosedEvent{window});
	}
}

Window* DisplayManager::CreateWindow()
{
	_windows.push_back(new Window (this, _windows.size()));
	return _windows.back();
}

Window* DisplayManager::GetWindow(unsigned id) const
{
	if (id < _windows.size())
		return _windows[id];

	return nullptr;
}

unsigned DisplayManager::GetNumWindows() const
{
	return _windows.size();
}

Kernel* DisplayManager::GetKernel() const
{
	return _kernel;
}

Factory<DisplayModeBase>& DisplayManager::GetDisplayModeFactory()
{
	return _displayModeFactory;
}

} //end of namespace JHL
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
