module;
#include "bembel/pch.h"
export module bembel.kernel.display:Window;

import glfw;

import bembel.base;
import bembel.kernel.assets;

import :Viewport;
import :DisplayMode;

namespace bembel::kernel {
using namespace bembel::base;

export class Window {
    using ViewportPtr    = std::unique_ptr<Viewport>;
    using DisplayModePtr = std::shared_ptr<DisplayModeBase>;

  public:
    Window(EventManager&, WindowId);
    Window(Window const&) = delete;
    Window(Window&&)      = default;
    ~Window();

    EventManager& getEventManager() { return m_event_mgr; }

    DisplayModePtr getDisplayMode() const { return m_display_mode; }
    void           setDisplayMode(DisplayModePtr val) { m_display_mode = val; }

    void init(xml::Element const* properties);

    void open(std::string_view titel);
    void close();

    WindowId getWindowID() const { return m_window_id; }

    bool isOpend() { return m_window_impl != nullptr; }

    glm::ivec2 getWindowSize() const;
    glm::ivec2 getFrameBufferSize() const;

    bool setIconify(bool b);
    bool setVisible(bool b);

    bool getShouldClose() const;
    bool setShouldClose(bool should_close);

    glm::vec3 const& getBackgroundColor() const { return m_background_color; }
    void             setBackgroundColor(glm::vec3 const& color) { m_background_color = color; }

    Viewport& createViewport(
        vec2 relativ_position,
        vec2 relativ_size,
        vec2 position_offset = {0.f, 0.f},
        vec2 size_offset     = {0.f, 0.f}
    );

    Viewport* getViewport(u32 index = 0) const {
        return index < m_viewports.size() ? m_viewports[index].get() : nullptr;
    }

    std::vector<ViewportPtr> const& getViewports() const { return m_viewports; }

    GLFWwindow* getGlfwWindow() const { return m_window_impl; }

    void makeContextCurent();
    void swapBuffers();

    void updateViewports(glm::ivec2 const& frambuffer_size) {
        for(auto& viewport : m_viewports) {
            viewport->updatePosition(frambuffer_size);
            viewport->updateSize(frambuffer_size);
        }
    }

    void handleEvent(SetCursorIconEvent const& event);

  private:
    EventManager& m_event_mgr;

    GLFWwindow*    m_window_impl = nullptr;
    DisplayModePtr m_display_mode;

    glm::vec3 m_background_color = {0.5f, 0.5f, 0.5f};

    std::vector<ViewportPtr> m_viewports;

    WindowId m_window_id;

    friend class DisplayManager;
};

} // namespace bembel::kernel
