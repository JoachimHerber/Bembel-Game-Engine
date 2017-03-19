/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-open-gl.h"

#include "window.h"
#include "display-manager.h"
#include "display-mode.h"

#include <cassert>

#include <glbinding/Binding.h>
#include <GLFW/glfw3.h>

/*============================================================================*/
/* IMPLEMENTATION			                                                  */
/*============================================================================*/
namespace bembel {

Window::Window(DisplayManager* display_system, unsigned id)
	: display_system_(display_system)
	, window_imp_(nullptr)
	, display_mode_(std::make_shared<WindowDisplayMode>())
	, window_id_(id)
{}

Window::~Window()
{
	Close();
}

void Window::Open(const std::string& titel)
{
	if( window_imp_ )
		return; // window already opend;

	window_imp_ = display_mode_->CreatWindow(titel, nullptr);
	glbinding::Binding::initialize();

	display_system_->OnWindowOpend(this);

	MakeContextCurent();
	glEnable(GL_FRAMEBUFFER_SRGB);
}

void Window::Close()
{
	if( !window_imp_ )
		return; // window is not opend;

	display_system_->OnWindowClosed(this);

	glfwDestroyWindow(window_imp_);
	window_imp_ = nullptr;
}

unsigned Window::GetWindowID() const
{
	return window_id_;
}

DisplayManager* Window::GetDisplayManager() const
{
	return display_system_;
}

GLFWwindow* Window::GetGlfwWindow() const
{
	return window_imp_;
}

bool Window::GetShouldClose() const
{
	if( !window_imp_ )
		return false;

	return int(GL_TRUE) == glfwWindowShouldClose(window_imp_);
}

bool Window::SetShouldClose(bool should_close)
{
	if( !window_imp_ )
		return false;

	glfwSetWindowShouldClose(window_imp_, should_close);
	return true;
}

glm::ivec2 Window::GetWindowSize() const
{
	glm::ivec2 size;
	glfwGetWindowSize(window_imp_, &size.x, &size.y);
	return size;
}

glm::ivec2 Window::GetFrameBufferSize() const
{
	glm::ivec2 size;
	glfwGetFramebufferSize(window_imp_, &size.x, &size.y);
	return size;
}

bool Window::SetIconify(bool b)
{
	if( b )
		glfwIconifyWindow(window_imp_);
	else
		glfwRestoreWindow(window_imp_);

	return true;
}

bool Window::SetVisible(bool b)
{
	if( b )
		glfwShowWindow(window_imp_);
	else
		glfwHideWindow(window_imp_);
	return true;
}

std::shared_ptr<DisplayModeBase> Window::GetDisplayMode() const
{
	return display_mode_;
}

void Window::SetDisplayMode(std::shared_ptr<DisplayModeBase> val)
{
	display_mode_ = val;
}

void Window::Init(const xml::Element* properties)
{
	if( !properties )
		return;

	std::string mode;
	auto* dispMode = properties->FirstChildElement("DisplayMode");
	if( xml::GetAttribute(dispMode, "mode", mode) )
	{
		display_mode_ =
			display_system_->GetDisplayModeFactory().CreateObject(mode, dispMode);
	}

	std::string title = "Bembel";
	xml::GetAttribute(properties, "title", title);
	Open(title);
}

bool Window::IsOpend()
{
	return window_imp_ != nullptr;
}

void Window::MakeContextCurent()
{
	glfwMakeContextCurrent(window_imp_);
	glbinding::Binding::useCurrentContext();
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(window_imp_);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
