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

#include "display-manager.h"

#include "window.h"
#include "../kernel.h"
#include "../events/display-events.h"
#include "../events/event-callbacks.h"

#include <glfw/glfw3.h>

#include <bembel-base/logging/logger.h>

/*============================================================================*/
/* IMPLEMENTATION			                                                  */
/*============================================================================*/
namespace bembel{

DisplayManager::DisplayManager(Kernel* kernel)
	: kernel_(kernel)
{
	//glfwSetMonitorCallback(glfw_callbacks::MonitorCallback);

	display_mode_factory_.RegisterDefaultObjectGenerator<WindowDisplayMode>("Windowed");
	display_mode_factory_.RegisterDefaultObjectGenerator<FullscreenDisplayMode>("Fullscreen");
}

DisplayManager::~DisplayManager()
{
	DeleteAllWindows();
}

bool DisplayManager::CreateWindows(const xml::Element* properties)
{
	if (!properties)
		return false;

	for (auto window_properties : xml::IterateChildElements(properties, "Window") )
	{
		windows_.push_back( std::make_unique<Window>(this, windows_.size()));
		windows_.back()->Init(window_properties);
	}
	return true;
}

void DisplayManager::CloseOpenWindows()
{
	while (!open_windows_.empty())
		open_windows_.front()->Close();

}

void DisplayManager::DeleteAllWindows()
{
	CloseOpenWindows();
	windows_.clear();
}


void DisplayManager::UpdateWindows()
{
	for (Window* window : open_windows_ )
	{
		window->MakeContextCurent();

		glClearColor(0.5f, 0.5f, 0.5f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		kernel_->GetEventManager()->Broadcast(WindowUpdateEvent{window});

		window->SwapBuffers();
	}
}

void DisplayManager::OnWindowOpend(Window* window)
{
	open_windows_.push_back(window);
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

	kernel_->GetEventManager()->Broadcast(WindowOpendEvent{window});
}

void DisplayManager::OnWindowClosed(Window* window)
{
	auto it = std::find(open_windows_.begin(), open_windows_.end(), window);
	if(it != open_windows_.end())
	{
		open_windows_.erase(it);
		kernel_->GetEventManager()->Broadcast(WindowClosedEvent{window});
	}
}

Window* DisplayManager::CreateWindow()
{
	windows_.push_back(std::make_unique<Window>(this, windows_.size()));
	return windows_.back().get();
}

Window* DisplayManager::GetWindow(unsigned id) const
{
	if (id < windows_.size())
		return windows_[id].get();

	return nullptr;
}

unsigned DisplayManager::GetNumWindows() const
{
	return windows_.size();
}

Kernel* DisplayManager::GetKernel() const
{
	return kernel_;
}

Factory<DisplayModeBase>& DisplayManager::GetDisplayModeFactory()
{
	return display_mode_factory_;
}

} //end of namespace JHL
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
