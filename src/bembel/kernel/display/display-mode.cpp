module;
// #define GLFW_INCLUDE_NONE
// #include <GLFW/glfw3.h>
module bembel.kernel.display;

import std;
import glfw;
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
    getAttribute(properties, "decorated", m_decorated);
    if(getAttribute(properties, "SizeLimit", "min_width", m_min_size.x)
       && getAttribute(properties, "SizeLimit", "min_height", m_min_size.y)
       && getAttribute(properties, "SizeLimit", "max_width", m_min_size.x)
       && getAttribute(properties, "SizeLimit", "max_height", m_min_size.y))
    {
        m_limit_size = true;
    }
    getAttribute(properties, "AspectRatio", "numerator", m_aspect_ratio_numer);
    getAttribute(properties, "AspectRatio", "denominator", m_aspect_ratio_denom);

    return true;
}

GLFWwindow* WindowDisplayMode::creatWindow(std::string_view title, GLFWwindow* shared_context) {
    glfw::setDefaultWindowHints();
    // GLFWwindow
    glfw::setWindowHint(glfw::RESIZABLE, m_resizable);
    glfw::setWindowHint(glfw::SRGB_CAPABLE, true);
    glfw::setWindowHint(glfw::DECORATED, m_decorated);

    std::string   title_str{title};
    glfw::Window* window =
        glfw::createWindow(m_width, m_height, title_str.c_str(), nullptr, shared_context);

    if(m_limit_size) {
        glfw::setWindowSizeLimits(window, m_min_size.x, m_min_size.y, m_max_size.x, m_max_size.y);
    }

    if(m_aspect_ratio_numer != 0 && m_aspect_ratio_denom != 0) {
        glfw::setWindowAspectRatio(window, m_aspect_ratio_numer, m_aspect_ratio_denom);
    }

    return window;
}
bool WindowDisplayMode::registerd =
    DisplayModeBase::getFactory().registerObjectGenerator<WindowDisplayMode>("Windowed");

FullscreenDisplayMode::FullscreenDisplayMode() {
    glfw::VideoMode const* mode = glfw::getVideoMode(glfw::getPrimaryMonitor());

    m_resolution.x = mode->width;
    m_resolution.y = mode->height;

    m_refresh_rate = mode->refreshRate;
}

glfw::Window* FullscreenDisplayMode::creatWindow(std::string_view title, GLFWwindow* shared_context) {
    glfw::setDefaultWindowHints();

    glfw::setWindowHint(glfw::RED_BITS, 8);
    glfw::setWindowHint(glfw::GREEN_BITS, 8);
    glfw::setWindowHint(glfw::BLUE_BITS, 8);
    glfw::setWindowHint(glfw::ALPHA_BITS, 8);
    glfw::setWindowHint(glfw::REFRESH_RATE, m_refresh_rate);

    std::string title_str{title};
    return glfw::createWindow(
        m_resolution.x, m_resolution.y, title_str.c_str(), glfwGetPrimaryMonitor(), shared_context
    );
}

bool FullscreenDisplayMode::configure(xml::Element const*) {
    return true;
}

bool FullscreenDisplayMode::registerd =
    DisplayModeBase::getFactory().registerObjectGenerator<FullscreenDisplayMode>("Fullscreen");

} // namespace bembel::kernel
