/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "DisplayMode.h"


#include <GLFW/glfw3.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

int WindowDisplayMode::GetWidth() const
{
	return _width;
}
void WindowDisplayMode::SetWidth(int value)
{
	_width = value;
}

int WindowDisplayMode::GetHeight() const
{
	return _height;
}
void WindowDisplayMode::SetHeight(int value)
{
	_height = value;
}

bool WindowDisplayMode::GetIsResizable() const
{
	return _resizable;
}
void WindowDisplayMode::SetIsResizable(bool value)
{
	_resizable = value;
}

GLFWwindow* WindowDisplayMode::CreatWindow(
	const std::string& title, 
	GLFWwindow* sharedContext)
{
	glfwDefaultWindowHints();

	glfwWindowHint(GLFW_RESIZABLE, _resizable);

	return glfwCreateWindow(
		_width, _height, title.c_str(), nullptr, sharedContext);
}

std::unique_ptr<WindowDisplayMode> 
	WindowDisplayMode::CreateInstance(const xml::Element* properties)
{
	auto mode = std::make_unique<WindowDisplayMode>();
	
	xml::GetAttribute(properties, "width",  mode->_width);
	xml::GetAttribute(properties, "height", mode->_height);
	xml::GetAttribute(properties, "resizable", mode->_resizable);

	return std::move(mode);
}

const std::string& WindowDisplayMode::GetTypeName()
{
	const static std::string typeName = "WindowDisplayMode";
	return typeName;
}

FullscreenDisplayMode::FullscreenDisplayMode()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	_resolution.x = mode->width;
	_resolution.y = mode->height;

	_refreshRate = mode->refreshRate;
}

glm::ivec2 FullscreenDisplayMode::GetResolution() const
{
	return _resolution;
}

void FullscreenDisplayMode::SetResolution(glm::ivec2 val)
{
	_resolution = val;
}

int FullscreenDisplayMode::GetRefreshRate() const
{
	return _refreshRate;
}

void FullscreenDisplayMode::SetRefreshRate(int val)
{
	_refreshRate = val;
}

GLFWwindow* FullscreenDisplayMode::CreatWindow(
	const std::string& title,
	GLFWwindow* sharedContext)
{
	glfwDefaultWindowHints();

	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_REFRESH_RATE, _refreshRate);

	return glfwCreateWindow(
		_resolution.x,
		_resolution.y, 
		title.c_str(), 
		glfwGetPrimaryMonitor(), 
		sharedContext
	);
}

std::unique_ptr<FullscreenDisplayMode> 
	FullscreenDisplayMode::CreateInstance(const xml::Element* properties)
{
	auto mode = std::make_unique<FullscreenDisplayMode>();

	return std::move(mode);
}

const std::string& FullscreenDisplayMode::GetTypeName()
{
	const static std::string typeName = "FullscreenDisplayMode";
	return typeName;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

