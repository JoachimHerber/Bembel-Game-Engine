/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "DisplayManager.h"

#include "Window.h"
#include "Events.h"
#include "EventCallbacks.h"

#include <BembelBase/Logging/Logger.h>

#include <glfw/glfw3.h>

/*============================================================================*/
/* IMPLEMENTATION			                                                  */
/*============================================================================*/
namespace bembel{

DisplayManager::DisplayManager(std::shared_ptr<EventManager> eventMgr)
	: _eventMgr(eventMgr)
{
	if (glfwInit() == GL_FALSE)
	{
		BEMBEL_LOG_ERROR() << "Failed to initialize GLFW" << std::endl;
		throw std::exception();
	}

	glfwSetMonitorCallback(glfw_callbacks::MonitorCallback);

	_displayModeFactory.RegisterDefaultObjectGenerator<WindowDisplayMode>("Windowed");
	_displayModeFactory.RegisterDefaultObjectGenerator<FullscreenDisplayMode>("Fullscreen");
}

DisplayManager::~DisplayManager()
{
	glfwTerminate();
}

bool DisplayManager::Init()
{
	return true;
}

bool DisplayManager::Init(const xml::Element* properties)
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

void DisplayManager::Shutdown()
{
	// close open windows
	while (!_openWindows.empty())
		_openWindows.front()->Close();

	for (Window* window : _windows)
		delete window;

	_windows.clear();

}

void DisplayManager::Update(double)
{
	glfwPollEvents();

	for (Window* window : _openWindows)
	{
		window->MakeContextCurent();

		glClearColor(0.5f, 0.5f, 0.5f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_eventMgr->Broadcast(WindowUpdateEvent{window});

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

	_eventMgr->Broadcast(WindowOpendEvent{window});
}

void DisplayManager::OnWindowClosed(Window* window)
{
	auto it = std::find(_openWindows.begin(), _openWindows.end(), window);
	if(it != _openWindows.end())
	{
		_openWindows.erase(it);
		_eventMgr->Broadcast(WindowClosedEvent{window});
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

EventManager* DisplayManager::GetEventManager() const
{
	return _eventMgr.get();
}

Factory<DisplayModeBase>& DisplayManager::GetDisplayModeFactory()
{
	return _displayModeFactory;
}

} //end of namespace JHL
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
