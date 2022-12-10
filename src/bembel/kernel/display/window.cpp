﻿module;
#include <glbinding/gl/gl.h>
// glbinding/gl/gl.h must be included first
#include <glbinding/glbinding.h>
module bembel.kernel.display;

import bembel.base;
import bembel.kernel.assets;

using namespace gl;

namespace bembel::kernel {
using namespace bembel::base;

namespace event_callbacks {
    Window* getWindow(GLFWwindow* glfw) {
        return static_cast<Window*>(glfwGetWindowUserPointer(glfw));
    }

    void windowPositionCallback(GLFWwindow* glfw, int x, int y) {
        auto window = getWindow(glfw);

        events::broadcast<WindowMovedEvent>(window, vec2{x, y});
    }
    void windowSizeCallback(GLFWwindow* glfw, int w, int h) {
        auto window = getWindow(glfw);

        events::broadcast<WindowResizeEvent>(window, vec2{w, h});
    }
    void windowCloseCallback(GLFWwindow* glfw) {
        auto window = getWindow(glfw);

        events::broadcast<WindowShouldCloseEvent>(window);
    }
    void windowRefreshCallback(GLFWwindow* glfw) {
        auto window = getWindow(glfw);
        // currently unused
    }
    void windowFocusCallback(GLFWwindow* glfw, int focused) {
        auto window = getWindow(glfw);

        if(focused)
            events::broadcast<WindowGainedFocusEvent>(window);
        else
            events::broadcast<WindowLostFocusEvent>(window);
    }
    void windowIconifyCallback(GLFWwindow* glfw, int iconified) {
        auto window = getWindow(glfw);
        if(iconified)
            events::broadcast<WindowIconifyedEvent>(window);
        else
            events::broadcast<WindowRestoredEvent>(window);
    }

    void framebufferSizeCallback(GLFWwindow* glfw, int w, int h) {
        auto window = getWindow(glfw);

        vec2 framebuffer_size(w, h);

        events::broadcast<FrameBufferResizeEvent>(window, framebuffer_size);
        window->updateViewports(framebuffer_size);
    }

    void keyCallback(GLFWwindow* glfw, int key_id, int scancode, int action, int mods) {
        auto window = getWindow(glfw);

        if(action == GLFW_PRESS)
            events::broadcast<KeyPressEvent>(window, key_id, scancode, mods);
        else if(action == GLFW_REPEAT)
            events::broadcast<KeyRepeatEvent>(window, key_id, scancode, mods);
        else if(action == GLFW_RELEASE)
            events::broadcast<KeyReleaseEvent>(window, key_id, scancode, mods);
    }

    void charCallback(GLFWwindow* glfw, unsigned int c) {
        auto window = getWindow(glfw);

        events::broadcast<TextInputEvent>(window, c);
    }
    void charModsCallback(GLFWwindow* glfw, unsigned int c, int mods) {
        auto window = getWindow(glfw);
        // currently unused
    }

    void mouseButtonCallback(GLFWwindow* glfw, int button_id, int action, int mods) {
        auto window = getWindow(glfw);

        if(action == GLFW_PRESS)
            events::broadcast<MouseButtonPressEvent>(window, button_id, mods);
        else if(action == GLFW_REPEAT)
            events::broadcast<MouseButtonRepeatEvent>(window, button_id, mods);
        else if(action == GLFW_RELEASE)
            events::broadcast<MouseButtonReleaseEvent>(window, button_id, mods);
    }

    void cursorPositionCallback(GLFWwindow* glfw, double x, double y) {
        auto window = getWindow(glfw);

        events::broadcast(CursorMovedEvent{window, glm::vec2(x, y)});

        auto& viewports = window->getViewports();
        for(auto it = viewports.rbegin(); it != viewports.rend(); ++it) {
            Viewport* vp = it->get();
            if(!vp->isEnabled()) continue;

            glm::vec2 pos = glm::vec2(x, y) - glm::vec2(vp->getPosition());
            pos.y         = vp->getSize().y - pos.y;
            vp->onCurserMove(pos);
        }
    }

    void cursorEnterCallback(GLFWwindow* glfw, int entered) {
        auto window = getWindow(glfw);

        if(entered)
            events::broadcast(CursorEnteredEvent{window});
        else
            events::broadcast(CursorLeftEvent{window});
    }

    void scrollCallback(GLFWwindow* glfw, double x, double y) {
        auto window = getWindow(glfw);

        events::broadcast(ScrollEvent{window, x, y});
    }

    void dropCallback(GLFWwindow* glfw, int count, char const** files) {
        auto window = getWindow(glfw);

        std::vector<std::string> file_names;
        for(int i = 0; i < count; ++i) file_names.push_back(files[i]);
        events::broadcast(FileDropEvent{window, std::move(file_names)});
    }

    void monitorCallback(GLFWmonitor* glfw, int) {
        //  Window*       window = GetWindow(glfw);
    }

} // namespace event_callbacks

Window::Window(WindowId id)
  : m_display_mode{std::make_shared<WindowDisplayMode>()}, m_window_id{id} {
    events::addHandler<SetCursorIconEvent>(this);
}

Window::~Window() {
    events::removeHandler<SetCursorIconEvent>(this);
    close();
}

void Window::init(base::xml::Element const* properties) {
    if(!properties) return;

    std::string mode;
    auto*       disp_mode = properties->FirstChildElement("DisplayMode");
    if(xml::getAttribute(disp_mode, "mode", mode)) {
        m_display_mode = DisplayModeBase::getFactory().createObject(mode);
        m_display_mode->configure(disp_mode);
    }

    xml::getAttribute(properties, "background_color", this->background_color);

    auto* viewports = properties->FirstChildElement("Viewports");
    if(viewports) {
        for(auto it : xml::IterateChildElements(viewports, "Viewport")) {
            vec2 relativ_position(0, 0);
            vec2 relativ_size(1, 1);
            vec2 position_offset(0, 0);
            vec2 size_offset(0, 0);
            xml::getAttribute(it, "relativ_position", relativ_position);
            xml::getAttribute(it, "relativ_size", relativ_size);
            xml::getAttribute(it, "position_offset", position_offset);
            xml::getAttribute(it, "size_offset", size_offset);

            m_viewports.push_back(std::make_unique<Viewport>(
                m_window_id, relativ_position, relativ_size, position_offset, size_offset
            ));
        }
    }

    std::string title = "Bembel";
    xml::getAttribute(properties, "title", title);
    this->open(title);
}

void Window::open(std::string_view titel) {
    if(m_window_impl) return; // window already opend;

    m_window_impl = m_display_mode->creatWindow(titel, nullptr);

    glbinding::initialize(glfwGetProcAddress, false);

    events::broadcast(WindowOpendEvent{this});

    glfwSetWindowUserPointer(m_window_impl, this);

    using namespace event_callbacks;
    glfwSetWindowPosCallback(m_window_impl, windowPositionCallback);
    glfwSetWindowSizeCallback(m_window_impl, windowSizeCallback);
    glfwSetWindowCloseCallback(m_window_impl, windowCloseCallback);
    glfwSetWindowRefreshCallback(m_window_impl, windowRefreshCallback);
    glfwSetWindowFocusCallback(m_window_impl, windowFocusCallback);
    glfwSetWindowIconifyCallback(m_window_impl, windowIconifyCallback);

    glfwSetFramebufferSizeCallback(m_window_impl, framebufferSizeCallback);

    glfwSetKeyCallback(m_window_impl, keyCallback);
    glfwSetCharCallback(m_window_impl, charCallback);
    glfwSetCharModsCallback(m_window_impl, charModsCallback);

    glfwSetMouseButtonCallback(m_window_impl, mouseButtonCallback);
    glfwSetCursorPosCallback(m_window_impl, cursorPositionCallback);
    glfwSetCursorEnterCallback(m_window_impl, cursorEnterCallback);
    glfwSetScrollCallback(m_window_impl, scrollCallback);

    glfwSetDropCallback(m_window_impl, dropCallback);

    makeContextCurent();
    glEnable(GL_FRAMEBUFFER_SRGB);

    updateViewports(getFrameBufferSize());
}

void Window::close() {
    if(!m_window_impl) return; // window is not opend;

    events::broadcast(WindowClosedEvent{this});

    glfwDestroyWindow(m_window_impl);
    m_window_impl = nullptr;
}

bool Window::getShouldClose() const {
    if(!m_window_impl) return false;

    return int(GL_TRUE) == glfwWindowShouldClose(m_window_impl);
}

bool Window::setShouldClose(bool should_close) {
    if(!m_window_impl) return false;

    glfwSetWindowShouldClose(m_window_impl, should_close);
    return true;
}

Viewport& Window::createViewport(
    vec2 relativ_position, vec2 relativ_size, vec2 position_offset, vec2 size_offset
) {
    m_viewports.push_back(std::make_unique<Viewport>(
        m_window_id, relativ_position, relativ_size, position_offset, size_offset
    ));
    return *m_viewports.back();
}

ivec2 Window::getWindowSize() const {
    ivec2 size;
    glfwGetWindowSize(m_window_impl, &size.x, &size.y);
    return size;
}

ivec2 Window::getFrameBufferSize() const {
    ivec2 size;
    glfwGetFramebufferSize(m_window_impl, &size.x, &size.y);
    return size;
}

bool Window::setIconify(bool b) {
    if(b)
        glfwIconifyWindow(m_window_impl);
    else
        glfwRestoreWindow(m_window_impl);

    return true;
}

bool Window::setVisible(bool b) {
    if(b)
        glfwShowWindow(m_window_impl);
    else
        glfwHideWindow(m_window_impl);
    return true;
}

void Window::makeContextCurent() {
    glfwMakeContextCurrent(m_window_impl);
    glbinding::useCurrentContext();
}

void Window::swapBuffers() {
    glfwSwapBuffers(m_window_impl);
}

void Window::handleEvent(SetCursorIconEvent const& event) {
    if(event.window_id == m_window_id) {
        if(event.cursor != nullptr) {
            glfwSetCursor(m_window_impl, event.cursor->getCursor());
            // Log().info("Using cursor {} ({:x})", event.cursor->getName(),
            // u64(event.cursor->getCursor()));
        } else {
            glfwSetCursor(m_window_impl, NULL);
        }
    }
}

} // namespace bembel::kernel