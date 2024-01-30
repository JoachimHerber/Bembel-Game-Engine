module;
#include <memory>
#include <string>
export module bembel.kernel.display:Window;

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
    Window(WindowId);
    Window(Window const&) = delete;
    Window(Window&&)      = default;
    ~Window();

    DisplayModePtr getDisplayMode() const { return m_display_mode; }
    void           setDisplayMode(Copy<DisplayModePtr> val) { m_display_mode = std::move(val); }

    void init(In<not_null_ptr<const xml::Element>> properties);

    void open(In<std::string_view> titel);
    void close();

    WindowId getWindowID() const { return m_window_id; }

    bool isOpend() { return m_window_impl != nullptr; }

    ivec2 getWindowPosition() const;
    void  setWindowPosition(In<ivec2> pos);

    ivec2 getWindowSize() const;
    void  setWindowSize(In<ivec2> size);

    ivec2 getFrameBufferSize() const;

    bool setVisible(bool b);

    void iconify();
    void maximize();
    void restore();

    bool getShouldClose() const;
    bool setShouldClose(bool should_close);

    Viewport& createViewport(
        vec2 relativ_position,
        vec2 relativ_size,
        vec2 position_offset = {0.f, 0.f},
        vec2 size_offset     = {0.f, 0.f}
    );
    Viewport& createViewport();

    Viewport* getViewport(u32 index = 0) const {
        return index < m_viewports.size() ? m_viewports[index].get() : nullptr;
    }

    std::vector<ViewportPtr> const& getViewports() const { return m_viewports; }

    GLFWwindow* getGlfwWindow() const { return m_window_impl; }

    void makeContextCurent();
    void swapBuffers();

    void updateViewports(In<ivec2> framebuffer_size) {
        for(auto& viewport : m_viewports) {
            viewport->updatePosition(framebuffer_size);
            viewport->updateSize(framebuffer_size);
        }
    }

    void handleEvent(In<SetCursorIconEvent> event);
    void handleEvent(In<SetCursorModeEvent> event);

  public:
    vec3 background_color = {0.5f, 0.5f, 0.5f};

  private:
    GLFWwindow*    m_window_impl = nullptr;
    DisplayModePtr m_display_mode;

    std::vector<ViewportPtr> m_viewports;

    WindowId m_window_id;

    friend class DisplayManager;
};

} // namespace bembel::kernel
