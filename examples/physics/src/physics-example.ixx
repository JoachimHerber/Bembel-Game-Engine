export module bembel.examples.physics;

import std;
import bembel;
import :CameraControle;

namespace bembel::examples::physics {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::graphics;
using namespace bembel::physics;
using namespace bembel::gui;

export class PhysicsExample : public kernel::Application<GraphicSystem, PhysicsSystem, GuiSystem> {
  public:
    PhysicsExample();
    ~PhysicsExample();

    virtual bool init(std::span<std::string_view> args) override;
    virtual void cleanup() override;

    void handleEvent(In<AppUpdateEvent>);
    void handleEvent(In<WindowShouldCloseEvent>);
    void handleEvent(In<FrameBufferResizeEvent>);
    void handleEvent(In<KeyPressEvent>);

  private:
    EventHandlerGuard<
        AppUpdateEvent,
        WindowShouldCloseEvent,
        FrameBufferResizeEvent,
        KeyPressEvent>
        m_guard = {this};

    GraphicalUserInterface* m_gui;
    std::shared_ptr<Scene>  m_scene;

    EntityID m_stirring_stick;
    double   m_rotation = 0;

    std::unique_ptr<CameraControle> m_camera;
};

} // namespace bembel::examples::physics
