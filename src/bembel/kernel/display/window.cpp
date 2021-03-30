#include "./window.hpp"

#include <bembel/kernel/open-gl.hpp>
// bembel/kernel/open-gl.hpp must be included before any file that includes gl.h

#include <GLFW/glfw3.h>
#include <glbinding/glbinding.h>

#include <bembel/base/logging/logger.hpp>
#include <bembel/kernel/core/kernel.hpp>
#include <bembel/kernel/events/event-manager.hpp>

#include "./cursor-icon.hpp"
#include "./display-manager.hpp"
#include "./display-mode.hpp"

namespace bembel::kernel {

Window::Window(DisplayManager& display_mgr, unsigned id)
: display_mgr(display_mgr)
, window_imp(nullptr)
, display_mode(std::make_shared<WindowDisplayMode>())
, window_id(id) {
    auto& event_mgr = this->display_mgr.getEventManager();
    event_mgr.addHandler<SetCursorIconEvent>(this);
}

Window::~Window() {
    auto& event_mgr = this->display_mgr.getEventManager();
    event_mgr.removeHandler<SetCursorIconEvent>(this);
    close();
}

void Window::open(const std::string& titel) {
    if(this->window_imp) return; // window already opend;

    this->window_imp = this->display_mode->creatWindow(titel, nullptr);

    glbinding::initialize(glfwGetProcAddress, false);

    this->display_mgr.onWindowOpend(this);

    makeContextCurent();
    glEnable(GL_FRAMEBUFFER_SRGB);

    this->updateViewports(this->getFrameBufferSize());
}

void Window::close() {
    if(!this->window_imp) return; // window is not opend;

    this->display_mgr.onWindowClosed(this);

    glfwDestroyWindow(this->window_imp);
    this->window_imp = nullptr;
}

unsigned Window::getWindowID() const {
    return this->window_id;
}

DisplayManager& Window::getDisplayManager() const {
    return this->display_mgr;
}

GLFWwindow* Window::getGlfwWindow() const {
    return this->window_imp;
}

bool Window::getShouldClose() const {
    if(!this->window_imp) return false;

    return int(GL_TRUE) == glfwWindowShouldClose(this->window_imp);
}

bool Window::setShouldClose(bool should_close) {
    if(!this->window_imp) return false;

    glfwSetWindowShouldClose(this->window_imp, should_close);
    return true;
}

Viewport* Window::getViewport(size_t index) const {
    return index < this->viewports.size() ? this->viewports[index].get() : nullptr;
}

const std::vector<std::unique_ptr<Viewport>>& Window::getViewports() const {
    return this->viewports;
}

glm::ivec2 Window::getWindowSize() const {
    glm::ivec2 size;
    glfwGetWindowSize(this->window_imp, &size.x, &size.y);
    return size;
}

glm::ivec2 Window::getFrameBufferSize() const {
    glm::ivec2 size;
    glfwGetFramebufferSize(this->window_imp, &size.x, &size.y);
    return size;
}

bool Window::setIconify(bool b) {
    if(b)
        glfwIconifyWindow(this->window_imp);
    else
        glfwRestoreWindow(this->window_imp);

    return true;
}

bool Window::setVisible(bool b) {
    if(b)
        glfwShowWindow(this->window_imp);
    else
        glfwHideWindow(this->window_imp);
    return true;
}

std::shared_ptr<DisplayModeBase> Window::getDisplayMode() const {
    return this->display_mode;
}

void Window::setDisplayMode(std::shared_ptr<DisplayModeBase> val) {
    this->display_mode = val;
}

void Window::init(const xml::Element* properties) {
    if(!properties) return;

    std::string mode;
    auto*       disp_mode = properties->FirstChildElement("DisplayMode");
    if(xml::getAttribute(disp_mode, "mode", mode)) {
        this->display_mode =
            this->display_mgr.getDisplayModeFactory().createObject(mode, disp_mode);
    }

    xml::getAttribute(properties, "background_color", this->background_color);

    auto* viewports = properties->FirstChildElement("Viewports");
    if(viewports) {
        for(auto it : base::xml::IterateChildElements(viewports, "Viewport")) {
            glm::vec2 relativ_position(0, 0);
            glm::vec2 relativ_size(1, 1);
            glm::vec2 position_offset(0, 0);
            glm::vec2 size_offset(0, 0);
            base::xml::getAttribute(it, "relativ_position", relativ_position);
            base::xml::getAttribute(it, "relativ_size", relativ_size);
            base::xml::getAttribute(it, "position_offset", position_offset);
            base::xml::getAttribute(it, "size_offset", size_offset);

            this->viewports.push_back(std::make_unique<Viewport>(
                this, relativ_position, relativ_size, position_offset, size_offset));
        }
    }

    std::string title = "Bembel";
    xml::getAttribute(properties, "title", title);
    this->open(title);
}

bool Window::isOpend() {
    return this->window_imp != nullptr;
}

void Window::makeContextCurent() {
    glfwMakeContextCurrent(this->window_imp);
    glbinding::useCurrentContext();
}

void Window::swapBuffers() {
    glfwSwapBuffers(this->window_imp);
}

void Window::updateViewports(const glm::ivec2& frambuffer_size) {
    for(auto& viewport : this->viewports) {
        viewport->updatePosition(frambuffer_size);
        viewport->updateSize(frambuffer_size);
    }
}

void Window::handleEvent(const SetCursorIconEvent& event) {
    if(event.window_id == this->window_id) {
        if(event.cursor != nullptr)
            glfwSetCursor(this->window_imp, event.cursor->getCursor());
        else
            glfwSetCursor(this->window_imp, NULL);
    }
}

} // namespace bembel::kernel
