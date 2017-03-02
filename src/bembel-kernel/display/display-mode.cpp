/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "display-mode.h"

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

	GLFWwindow* window =  glfwCreateWindow(
		_width, _height, title.c_str(), nullptr, sharedContext);

	if(_limitSize)
		glfwSetWindowSizeLimits(window, _minSize.x, _minSize.y, _maxSize.x, _maxSize.y);

	if (_aspectRatioNumer!=0 && _aspectRatioDenom !=0)
		glfwSetWindowAspectRatio(window, _aspectRatioNumer, _aspectRatioDenom);

	return window;
}

std::unique_ptr<WindowDisplayMode> 
	WindowDisplayMode::CreateInstance(const xml::Element* properties)
{
	auto mode = std::make_unique<WindowDisplayMode>();
	
	xml::GetAttribute(properties, "Size", "width",  mode->_width);
	xml::GetAttribute(properties, "Size", "height", mode->_height);
	xml::GetAttribute(properties, "resizable", mode->_resizable);
	if( xml::GetAttribute(properties, "SizeLimit", "min_width",  mode->_minSize.x) &&
	    xml::GetAttribute(properties, "SizeLimit", "min_height", mode->_minSize.y) &&
	    xml::GetAttribute(properties,  "SizeLimit", "max_width",  mode->_minSize.x) &&
	    xml::GetAttribute(properties,  "SizeLimit", "max_height", mode->_minSize.y) )
	{
		mode->_limitSize = true;
	}
	xml::GetAttribute(properties, "AspectRatio", "numerator", mode->_aspectRatioNumer);
	xml::GetAttribute(properties, "AspectRatio", "denominator", mode->_aspectRatioDenom);

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

