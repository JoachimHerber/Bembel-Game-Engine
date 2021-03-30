#include "./display-mode.hpp"

#include <GLFW/glfw3.h>

namespace bembel::kernel {

int WindowDisplayMode::getWidth() const {
    return this->width;
}
void WindowDisplayMode::setWidth(int value) {
    this->width = value;
}

int WindowDisplayMode::getHeight() const {
    return this->height;
}
void WindowDisplayMode::setHeight(int value) {
    this->height = value;
}

bool WindowDisplayMode::getIsResizable() const {
    return this->resizable;
}
void WindowDisplayMode::setIsResizable(bool value) {
    this->resizable = value;
}

GLFWwindow* WindowDisplayMode::creatWindow(const std::string& title, GLFWwindow* shared_context) {
    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_RESIZABLE, this->resizable);
    glfwWindowHint(GLFW_SRGB_CAPABLE, true);

    GLFWwindow* window =
        glfwCreateWindow(this->width, this->height, title.c_str(), nullptr, shared_context);

    if(this->limit_size) {
        glfwSetWindowSizeLimits(
            window, this->min_size.x, this->min_size.y, this->max_size.x, this->max_size.y);
    }

    if(this->aspect_ratio_numer != 0 && this->aspect_ratio_denom != 0) {
        glfwSetWindowAspectRatio(window, this->aspect_ratio_numer, this->aspect_ratio_denom);
    }

    return window;
}

std::unique_ptr<WindowDisplayMode> WindowDisplayMode::createInstance(
    const base::xml::Element* properties) {
    auto mode = std::make_unique<WindowDisplayMode>();

    using namespace base::xml;

    getAttribute(properties, "Size", "width", mode->width);
    getAttribute(properties, "Size", "height", mode->height);
    getAttribute(properties, "resizable", mode->resizable);
    if(getAttribute(properties, "SizeLimit", "min_width", mode->min_size.x)
       && getAttribute(properties, "SizeLimit", "min_height", mode->min_size.y)
       && getAttribute(properties, "SizeLimit", "max_width", mode->min_size.x)
       && getAttribute(properties, "SizeLimit", "max_height", mode->min_size.y)) {
        mode->limit_size = true;
    }
    getAttribute(properties, "AspectRatio", "numerator", mode->aspect_ratio_numer);
    getAttribute(properties, "AspectRatio", "denominator", mode->aspect_ratio_denom);

    return std::move(mode);
}

const std::string& WindowDisplayMode::getTypeName() {
    const static std::string type_name = "WindowDisplayMode";
    return type_name;
}

FullscreenDisplayMode::FullscreenDisplayMode() {
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    this->resolution.x = mode->width;
    this->resolution.y = mode->height;

    this->refresh_rate = mode->refreshRate;
}

glm::ivec2 FullscreenDisplayMode::getResolution() const {
    return this->resolution;
}

void FullscreenDisplayMode::setResolution(glm::ivec2 val) {
    this->resolution = val;
}

int FullscreenDisplayMode::getRefreshRate() const {
    return this->refresh_rate;
}

void FullscreenDisplayMode::setRefreshRate(int val) {
    this->refresh_rate = val;
}

GLFWwindow* FullscreenDisplayMode::creatWindow(
    const std::string& title, GLFWwindow* shared_context) {
    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_REFRESH_RATE, this->refresh_rate);

    return glfwCreateWindow(
        this->resolution.x,
        this->resolution.y,
        title.c_str(),
        glfwGetPrimaryMonitor(),
        shared_context);
}

std::unique_ptr<FullscreenDisplayMode> FullscreenDisplayMode::createInstance(
    const base::xml::Element* properties) {
    auto mode = std::make_unique<FullscreenDisplayMode>();

    return std::move(mode);
}

const std::string& FullscreenDisplayMode::getTypeName() {
    const static std::string type_name = "FullscreenDisplayMode";
    return type_name;
}

} // namespace bembel::kernel
