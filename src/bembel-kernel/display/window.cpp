/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelOpenGL.h>
#include <glbinding/Binding.h>

#include <GLFW/glfw3.h>

#include "DisplayManager.h"
#include "Window.h"
#include "DisplayMode.h"

#include <cassert>

/*============================================================================*/
/* IMPLEMENTATION			                                                  */
/*============================================================================*/
namespace bembel{

Window::Window(DisplayManager* displaySys, unsigned id)
	: _displaySys(displaySys)
	, _windowImp(nullptr)
	, _displayMode(std::make_shared<WindowDisplayMode>())
	, _windowID(id)
{}

Window::~Window()
{
	Close();
}

void Window::Open(const std::string& titel)
{
	if (_windowImp)
		return; // window already opend;

	_windowImp = _displayMode->CreatWindow(titel, nullptr);
	glbinding::Binding::initialize();

	_displaySys->OnWindowOpend(this);

	MakeContextCurent();
	glEnable(GL_FRAMEBUFFER_SRGB);
}
void Window::Close()
{
	if (!_windowImp)
		return; // window is not opend;

	_displaySys->OnWindowClosed(this);

	glfwDestroyWindow(_windowImp);
	_windowImp = nullptr;
}

unsigned Window::GetWindowID() const
{
	return _windowID;
}

DisplayManager* Window::GetDisplayManager() const
{
	return _displaySys;
}

GLFWwindow* Window::GetGlfwWindow() const
{
	return _windowImp;
}

bool Window::GetShouldClose() const
{
	if (!_windowImp)
		return false;

	return int(GL_TRUE) == glfwWindowShouldClose(_windowImp);
}

bool Window::SetShouldClose(bool bShouldClose)
{
	if (!_windowImp)
		return false;

	glfwSetWindowShouldClose(_windowImp, bShouldClose);
	return true;
}

glm::ivec2 Window::GetWindowSize() const
{
	glm::ivec2 size;
	glfwGetWindowSize(_windowImp, &size.x, &size.y);
	return size;
}

glm::ivec2 Window::GetFrameBufferSize() const
{
	glm::ivec2 size;
	glfwGetFramebufferSize(_windowImp, &size.x, &size.y);
	return size;
}

bool Window::SetIconify(bool b)
{
	if (b)
		glfwIconifyWindow(_windowImp);
	else
		glfwRestoreWindow(_windowImp);

	return true;
}

bool Window::SetVisible(bool b)
{
	if (b)
		glfwShowWindow(_windowImp);
	else
		glfwHideWindow(_windowImp);
	return true;
}

std::shared_ptr<DisplayModeBase> Window::GetDisplayMode() const
{
	return _displayMode;
}

void Window::SetDisplayMode(std::shared_ptr<DisplayModeBase> val)
{
	_displayMode = val;
}

void Window::Init(const xml::Element* properties)
{
	if (!properties)
		return;

	std::string mode;
	auto* dispMode = properties->FirstChildElement("DisplayMode");
	if (xml::GetAttribute(dispMode, "mode", mode) )
	{
		_displayMode = 
			_displaySys->GetDisplayModeFactory().CreateObject(mode, dispMode);
	}

	std::string title = "Bembel";
	xml::GetAttribute(properties, "title", title);
	Open(title);
}

bool Window::IsOpend()
{
	return _windowImp != nullptr;
}

void Window::MakeContextCurent()
{
	glfwMakeContextCurrent(_windowImp);
	glbinding::Binding::useCurrentContext();
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(_windowImp);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
