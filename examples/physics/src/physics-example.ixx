module;
#include <filesystem>
#include <string_view>
export module bembel.examples.physics;

import bembel;
import :CameraControle;

namespace bembel::examples::physics {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::graphics;
using namespace bembel::physics;
using namespace bembel::gui;

export class PhysicsExample : public kernel::Application {
  public:
    PhysicsExample();
    ~PhysicsExample();

    virtual bool init() override;
    virtual void cleanup() override;

    virtual void update(double time) override;

    void handleEvent(In<WindowShouldCloseEvent>);
    void handleEvent(In<FrameBufferResizeEvent>);

  private:
    GraphicSystem* m_graphic_system;
    PhysicsSystem* m_physics_system;
    GuiSystem*     m_gui_system;

    GraphicalUserInterface* m_gui;
    std::shared_ptr<Scene>  m_scene;

    EntityID m_stirring_stick;
    double   m_rotation = 0;

    std::unique_ptr<CameraControle> m_camera;
};

} // namespace bembel::examples::physics
