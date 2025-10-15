module;
module bembel.kernel.display;

import std;
import glfw;
import glbinding;
import bembel.base;
import bembel.kernel.assets;

// using namespace gl;

namespace bembel::kernel {
using namespace bembel::base;

using namespace std::string_view_literals;

namespace event_callbacks {
    Window* getWindow(glfw::Window* glfw) {
        return static_cast<Window*>(glfw::getWindowUserPointer(glfw));
    }

    void windowPositionCallback(glfw::Window* glfw, int x, int y) {
        auto window = getWindow(glfw);

        events::broadcast<WindowMovedEvent>(window, vec2{x, y});
    }
    void windowSizeCallback(glfw::Window* glfw, int w, int h) {
        auto window = getWindow(glfw);

        events::broadcast<WindowResizeEvent>(window, vec2{w, h});
    }
    void windowCloseCallback(glfw::Window* glfw) {
        auto window = getWindow(glfw);

        events::broadcast<WindowShouldCloseEvent>(window);
    }
    void windowRefreshCallback(glfw::Window* glfw) {
        auto window = getWindow(glfw);
        // currently unused
    }
    void windowFocusCallback(glfw::Window* glfw, int focused) {
        auto window = getWindow(glfw);

        if(focused)
            events::broadcast<WindowGainedFocusEvent>(window);
        else
            events::broadcast<WindowLostFocusEvent>(window);
    }
    void windowIconifyCallback(glfw::Window* glfw, int iconified) {
        auto window = getWindow(glfw);
        if(iconified)
            events::broadcast<WindowIconifyedEvent>(window);
        else
            events::broadcast<WindowRestoredEvent>(window);
    }

    void framebufferSizeCallback(glfw::Window* glfw, int w, int h) {
        auto window = getWindow(glfw);

        ivec2 framebuffer_size(w, h);

        events::broadcast<FrameBufferResizeEvent>(window, framebuffer_size);
        window->updateViewports(framebuffer_size);
    }

    void keyCallback(glfw::Window* glfw, int key_id, int scancode, int action, int mods) {
        auto window = getWindow(glfw);

        if(action == glfw::PRESS)
            events::broadcast<KeyPressEvent>(window, key_id, scancode, mods);
        else if(action == glfw::REPEAT)
            events::broadcast<KeyRepeatEvent>(window, key_id, scancode, mods);
        else if(action == glfw::RELEASE)
            events::broadcast<KeyReleaseEvent>(window, key_id, scancode, mods);
    }

    void charCallback(glfw::Window* glfw, unsigned int c) {
        auto window = getWindow(glfw);

        events::broadcast<TextInputEvent>(window, c);
    }
    void charModsCallback(glfw::Window* glfw, unsigned int c, int mods) {
        auto window = getWindow(glfw);
        // currently unused
    }

    void mouseButtonCallback(glfw::Window* glfw, int button_id, int action, int mods) {
        auto window = getWindow(glfw);

        if(action == glfw::PRESS)
            events::broadcast<MouseButtonPressEvent>(window, button_id, mods);
        else if(action == glfw::REPEAT)
            events::broadcast<MouseButtonRepeatEvent>(window, button_id, mods);
        else if(action == glfw::RELEASE)
            events::broadcast<MouseButtonReleaseEvent>(window, button_id, mods);
    }

    void cursorPositionCallback(glfw::Window* glfw, double x, double y) {
        auto window = getWindow(glfw);

        events::broadcast(CursorMovedEvent{window, vec2(x, y)});

        auto& viewports = window->getViewports();
        for(auto it = viewports.rbegin(); it != viewports.rend(); ++it) {
            Viewport* vp = it->get();
            if(!vp->isEnabled()) continue;

            vec2 pos = vec2(x, y) - vec2(vp->getPosition());
            pos.y    = vp->getSize().y - pos.y;
            vp->onCurserMove(pos);
        }
    }

    void cursorEnterCallback(glfw::Window* glfw, int entered) {
        auto window = getWindow(glfw);

        if(entered)
            events::broadcast(CursorEnteredEvent{window});
        else
            events::broadcast(CursorLeftEvent{window});
    }

    void scrollCallback(glfw::Window* glfw, double x, double y) {
        auto window = getWindow(glfw);

        events::broadcast(ScrollEvent{window, x, y});
    }

    void dropCallback(glfw::Window* glfw, int count, char const** files) {
        auto window = getWindow(glfw);

        std::vector<std::filesystem::path> file_paths;
        for(int i = 0; i < count; ++i) file_paths.emplace_back(files[i]);
        events::broadcast(FileDropEvent{window, std::move(file_paths)});
    }

    void monitorCallback(glfw::Monitor* glfw, int) {
        //  Window*       window = GetWindow(glfw);
    }

} // namespace event_callbacks

Window::Window(WindowId id)
  : m_display_mode{std::make_shared<WindowDisplayMode>()}
  , m_window_id{id} {}

Window::~Window() {
    close();
}

void Window::init(In<not_null_ptr<const xml::Element>> properties, In<Window*> shared_context) {
    if(!properties) return;

    std::string mode;
    auto*       disp_mode = properties->FirstChildElement("DisplayMode");
    if(xml::getAttribute(disp_mode, "mode", mode)) {
        m_display_mode = DisplayModeBase::getFactory().createObject(mode);
        m_display_mode->configure(disp_mode);
    }

    xml::getAttribute<vec3>(properties, "background_color"sv, this->background_color);

    auto* viewports = properties->FirstChildElement("Viewports");
    if(viewports) {
        for(auto it : xml::IterateChildElements(viewports, "Viewport")) {
            auto relativ_position = xml::getAttribute<vec2>(it, "relativ_position");
            auto relativ_size     = xml::getAttribute<vec2>(it, "relativ_size");
            auto position_offset  = xml::getAttribute<vec2>(it, "position_offset");
            auto size_offset      = xml::getAttribute<vec2>(it, "size_offset");
            m_viewports.push_back(std::make_unique<Viewport>(
                m_window_id, relativ_position, relativ_size, position_offset, size_offset
            ));
        }
    }
    std::string title = "Bembel";
    xml::getAttribute(properties, "title", title);
    this->open(title, shared_context);
}

void Window::open(std::string_view titel, In<Window*> shared_context) {
    if(m_window_impl) return; // window already opend;

    m_window_impl = m_display_mode->creatWindow(
        titel, shared_context ? shared_context->getGlfwWindow() : nullptr
    );

    glbinding::initialize(false);

    events::broadcast(WindowOpendEvent{this});

    glfw::setWindowUserPointer(m_window_impl, this);

    using namespace event_callbacks;
    glfw::setWindowPosCallback(m_window_impl, windowPositionCallback);
    glfw::setWindowSizeCallback(m_window_impl, windowSizeCallback);
    glfw::setWindowCloseCallback(m_window_impl, windowCloseCallback);
    glfw::setWindowRefreshCallback(m_window_impl, windowRefreshCallback);
    glfw::setWindowFocusCallback(m_window_impl, windowFocusCallback);
    glfw::setWindowIconifyCallback(m_window_impl, windowIconifyCallback);

    glfw::setFramebufferSizeCallback(m_window_impl, framebufferSizeCallback);

    glfw::setKeyCallback(m_window_impl, keyCallback);
    glfw::setCharCallback(m_window_impl, charCallback);
    glfw::setCharModsCallback(m_window_impl, charModsCallback);

    glfw::setMouseButtonCallback(m_window_impl, mouseButtonCallback);
    glfw::setCursorPosCallback(m_window_impl, cursorPositionCallback);
    glfw::setCursorEnterCallback(m_window_impl, cursorEnterCallback);
    glfw::setScrollCallback(m_window_impl, scrollCallback);

    glfw::setDropCallback(m_window_impl, dropCallback);

    makeContextCurent();
    gl::glEnable(gl::GLenum::GL_FRAMEBUFFER_SRGB);

    updateViewports(getFrameBufferSize());
}

void Window::close() {
    if(!m_window_impl) return; // window is not opend;

    events::broadcast(WindowClosedEvent{this});

    glfw::destroyWindow(m_window_impl);
    m_window_impl = nullptr;
}

bool Window::getShouldClose() const {
    if(!m_window_impl) return false;

    return int(gl::GL_TRUE) == glfw::getWindowShouldClose(m_window_impl);
}

bool Window::setShouldClose(bool should_close) {
    if(!m_window_impl) return false;

    glfw::setWindowShouldClose(m_window_impl, should_close);
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
Viewport& Window::createViewport() {
    m_viewports.push_back(std::make_unique<Viewport>(m_window_id));
    return *m_viewports.back();
}

ivec2 Window::getWindowPosition() const {
    ivec2 pos;
    glfw::getWindowPos(m_window_impl, &pos.x, &pos.y);
    return pos;
}

void Window::setWindowPosition(In<ivec2> pos) {
    glfw::setWindowPos(m_window_impl, pos.x, pos.y);
}

ivec2 Window::getWindowSize() const {
    ivec2 size;
    glfw::getWindowSize(m_window_impl, &size.x, &size.y);
    return size;
}

void Window::setWindowSize(In<ivec2> size) {
    glfw::setWindowSize(m_window_impl, size.x, size.y);
}

ivec2 Window::getFrameBufferSize() const {
    ivec2 size;
    glfw::getFramebufferSize(m_window_impl, &size.x, &size.y);
    return size;
}

void Window::iconify() {
    glfw::iconifyWindow(m_window_impl);
}

void Window::maximize() {
    glfw::maximizeWindow(m_window_impl);
}

void Window::restore() {
    glfw::restoreWindow(m_window_impl);
}

bool Window::setVisible(bool b) {
    if(b)
        glfw::showWindow(m_window_impl);
    else
        glfw::hideWindow(m_window_impl);
    return true;
}

void Window::makeContextCurent() {
    glfw::makeContextCurrent(m_window_impl);
    glbinding::useCurrentContext();
}

void Window::swapBuffers() {
    glfw::swapBuffers(m_window_impl);
}

void Window::handleEvent(In<SetCursorIconEvent> event) {
    if(event.window_id == m_window_id) {
        if(event.cursor != nullptr) {
            glfw::setCursor(m_window_impl, event.cursor->getCursor());
            // Log().info("Using cursor {} ({:x})", event.cursor->getName(),
            // u64(event.cursor->getCursor()));
        } else {
            glfw::setCursor(m_window_impl, nullptr);
        }
    }
}

void Window::handleEvent(In<SetCursorModeEvent> event) {
    if(event.window_id == m_window_id) {
        switch(event.mode) {
            case CursorMode::NORMAL:
                glfw::setInputMode(m_window_impl, glfw::CURSOR, glfw::CURSOR_NORMAL);
                return;
            case CursorMode::HIDDEN:
                glfw::setInputMode(m_window_impl, glfw::CURSOR, glfw::CURSOR_HIDDEN);
                return;
            case CursorMode::DISABLED:
                glfw::setInputMode(m_window_impl, glfw::CURSOR, glfw::CURSOR_DISABLED);
                return;
        }
    }
}

} // namespace bembel::kernel
