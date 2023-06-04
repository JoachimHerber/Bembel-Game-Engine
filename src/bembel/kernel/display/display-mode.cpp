module;
#include <string>
#include <string_view>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
module bembel.kernel.display;

import bembel.base;

namespace bembel::kernel {
using namespace bembel::base;

Factory<DisplayModeBase>& DisplayModeBase::getFactory() {
    static Factory<DisplayModeBase> factory;
    return factory;
}

bool WindowDisplayMode::configure(xml::Element const* properties) {
    using namespace base::xml;

    getAttribute(properties, "Size", "width", m_width);
    getAttribute(properties, "Size", "height", m_height);
    getAttribute(properties, "resizable", m_resizable);
    if(getAttribute(properties, "SizeLimit", "min_width", m_min_size.x)
       && getAttribute(properties, "SizeLimit", "min_height", m_min_size.y)
       && getAttribute(properties, "SizeLimit", "max_width", m_min_size.x)
       && getAttribute(properties, "SizeLimit", "max_height", m_min_size.y)) {
        m_limit_size = true;
    }
    getAttribute(properties, "AspectRatio", "numerator", m_aspect_ratio_numer);
    getAttribute(properties, "AspectRatio", "denominator", m_aspect_ratio_denom);

    return true;
}

GLFWwindow* WindowDisplayMode::creatWindow(std::string_view title, GLFWwindow* shared_context) {
    glfwDefaultWindowHints();
    // GLFWwindow
    glfwWindowHint(GLFW_RESIZABLE, m_resizable);
    glfwWindowHint(GLFW_SRGB_CAPABLE, true);
    glfwWindowHint(GLFW_DECORATED, m_decorated);

    std::string title_str{title};
    GLFWwindow* window =
        glfwCreateWindow(m_width, m_height, title_str.c_str(), nullptr, shared_context);

    if(m_limit_size) {
        glfwSetWindowSizeLimits(window, m_min_size.x, m_min_size.y, m_max_size.x, m_max_size.y);
    }

    if(m_aspect_ratio_numer != 0 && m_aspect_ratio_denom != 0) {
        glfwSetWindowAspectRatio(window, m_aspect_ratio_numer, m_aspect_ratio_denom);
    }

    return window;
}
bool WindowDisplayMode::registerd =
    DisplayModeBase::getFactory().registerObjectGenerator<WindowDisplayMode>("Windowed");

FullscreenDisplayMode::FullscreenDisplayMode() {
    GLFWvidmode const* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    m_resolution.x = mode->width;
    m_resolution.y = mode->height;

    m_refresh_rate = mode->refreshRate;
}

GLFWwindow* FullscreenDisplayMode::creatWindow(std::string_view title, GLFWwindow* shared_context) {
    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_REFRESH_RATE, m_refresh_rate);

    std::string title_str{title};
    return glfwCreateWindow(
        m_resolution.x, m_resolution.y, title_str.c_str(), glfwGetPrimaryMonitor(), shared_context
    );
}

bool FullscreenDisplayMode::configure(xml::Element const*) {
    return true;
}

bool FullscreenDisplayMode::registerd =
    DisplayModeBase::getFactory().registerObjectGenerator<FullscreenDisplayMode>("Fullscreen");

} // namespace bembel::kernel
