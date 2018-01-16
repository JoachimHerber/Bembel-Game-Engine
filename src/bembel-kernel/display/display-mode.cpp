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

#include "display-mode.h"

#include <GLFW/glfw3.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

int WindowDisplayMode::GetWidth() const
{
	return width_;
}
void WindowDisplayMode::SetWidth(int value)
{
	width_ = value;
}

int WindowDisplayMode::GetHeight() const
{
	return height_;
}
void WindowDisplayMode::SetHeight(int value)
{
	height_ = value;
}

bool WindowDisplayMode::GetIsResizable() const
{
	return resizable_;
}
void WindowDisplayMode::SetIsResizable(bool value)
{
	resizable_ = value;
}

GLFWwindow* WindowDisplayMode::CreatWindow(
	const std::string& title,
	GLFWwindow* shared_context)
{
	glfwDefaultWindowHints();

	glfwWindowHint(GLFW_RESIZABLE, resizable_);
	glfwWindowHint(GLFW_SRGB_CAPABLE, true);

	GLFWwindow* window = glfwCreateWindow(
		width_, height_, title.c_str(), nullptr, shared_context);

	if( limit_size_ )
	{
		glfwSetWindowSizeLimits(
			window, min_size_.x, min_size_.y, max_size_.x, max_size_.y);
	}

	if( aspect_ratio_numer_!=0 && aspect_ratio_denom_ !=0 )
	{
		glfwSetWindowAspectRatio(
			window, aspect_ratio_numer_, aspect_ratio_denom_);
	}

	return window;
}

std::unique_ptr<WindowDisplayMode>
WindowDisplayMode::CreateInstance(const xml::Element* properties)
{
	auto mode = std::make_unique<WindowDisplayMode>();

	xml::GetAttribute(properties, "Size", "width", mode->width_);
	xml::GetAttribute(properties, "Size", "height", mode->height_);
	xml::GetAttribute(properties, "resizable", mode->resizable_);
	if( xml::GetAttribute(properties, "SizeLimit", "min_width", mode->min_size_.x) &&
		xml::GetAttribute(properties, "SizeLimit", "min_height", mode->min_size_.y) &&
		xml::GetAttribute(properties, "SizeLimit", "max_width", mode->min_size_.x) &&
		xml::GetAttribute(properties, "SizeLimit", "max_height", mode->min_size_.y) )
	{
		mode->limit_size_ = true;
	}
	xml::GetAttribute(properties, "AspectRatio", "numerator", mode->aspect_ratio_numer_);
	xml::GetAttribute(properties, "AspectRatio", "denominator", mode->aspect_ratio_denom_);

	return std::move(mode);
}

const std::string& WindowDisplayMode::GetTypeName()
{
	const static std::string type_name = "WindowDisplayMode";
	return type_name;
}

FullscreenDisplayMode::FullscreenDisplayMode()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	resolution_.x = mode->width;
	resolution_.y = mode->height;

	refresh_rate_ = mode->refreshRate;
}

glm::ivec2 FullscreenDisplayMode::GetResolution() const
{
	return resolution_;
}

void FullscreenDisplayMode::SetResolution(glm::ivec2 val)
{
	resolution_ = val;
}

int FullscreenDisplayMode::GetRefreshRate() const
{
	return refresh_rate_;
}

void FullscreenDisplayMode::SetRefreshRate(int val)
{
	refresh_rate_ = val;
}

GLFWwindow* FullscreenDisplayMode::CreatWindow(
	const std::string& title,
	GLFWwindow* shared_context)
{
	glfwDefaultWindowHints();

	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate_);

	return glfwCreateWindow(
		resolution_.x,
		resolution_.y,
		title.c_str(),
		glfwGetPrimaryMonitor(),
		shared_context
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
	const static std::string type_name = "FullscreenDisplayMode";
	return type_name;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

