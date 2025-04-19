export module bembel.examples.gm_helper;

import std;
import bembel;

namespace bembel::examples::gm_helper {
using namespace bembel::base;
using namespace bembel::kernel;

export class Application
  : public kernel::Application<GraphicSystem, PhysicsSystem, ParticleSystem, GuiSystem>
  , public imgui::View {
  public:
    Application();
    ~Application();

    virtual bool init(std::span<std::string_view> args) override;
    virtual void cleanup() override;

    void handleEvent(In<WindowShouldCloseEvent>);
    void handleEvent(In<KeyReleaseEvent>);
    void handleEvent(In<MouseButtonPressEvent>);
    void handleEvent(In<MouseButtonReleaseEvent>);
    void handleEvent(In<CursorMovedEvent>);
    void handleEvent(In<ScrollEvent>);

  private:
    virtual void drawUI(In<ivec2> viewport_position, In<uvec2> viewport_size) override;

  private:
    EventHandlerGuard<
        WindowShouldCloseEvent,
        KeyReleaseEvent,
        MouseButtonPressEvent,
        MouseButtonReleaseEvent,
        CursorMovedEvent,
        ScrollEvent>
        m_guard{this};

    Window* m_primary_window;
    Window* m_secondary_window;
    vec2    m_cursor_pos;

    std::shared_ptr<Camera> m_primary_camera   = std::make_shared<Camera>();
    std::shared_ptr<Camera> m_secondary_camera = std::make_shared<Camera>();

    bool  m_dragging_active = false;
    float m_primary_zoom    = 1.0f;
    float m_secondary_zoom  = 5.0f;
};

} // namespace bembel::examples::gm_helper
