module;
#include <memory>
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
    void           setDisplayMode(DisplayModePtr val) { m_display_mode = val; }

    void init(xml::Element const* properties);

    void open(std::string_view titel);
    void close();

    WindowId getWindowID() const { return m_window_id; }

    bool isOpend() { return m_window_impl != nullptr; }

    ivec2 getWindowSize() const;
    ivec2 getFrameBufferSize() const;

    bool setIconify(bool b);
    bool setVisible(bool b);

    bool getShouldClose() const;
    bool setShouldClose(bool should_close);

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

    void updateViewports(In<ivec2> frambuffer_size) {
        for(auto& viewport : m_viewports) {
            viewport->updatePosition(frambuffer_size);
            viewport->updateSize(frambuffer_size);
        }
    }

    void handleEvent(SetCursorIconEvent const& event);

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
