#ifndef BEMBEL_RENDERING_EXAMPLE_H
#define BEMBEL_RENDERING_EXAMPLE_H

#include <bembel/graphics/graphic-system.hpp>
#include <bembel/kernel/core/application.hpp>
#include <bembel/kernel/events/display-events.hpp>
#include <bembel/kernel/scene/scene.hpp>

#include "camera-controle.hpp"

namespace bembel {

class RenderingExample : public kernel::Application {
  public:
    RenderingExample();
    ~RenderingExample();

    virtual bool init() override;
    virtual void cleanup() override;

    virtual void update(double time) override;

    void handleEvent(const kernel::WindowShouldCloseEvent&);

  private:
    graphics::GraphicSystem* graphic_system;

    std::unique_ptr<CameraControle> camera;

    std::vector<std::shared_ptr<kernel::Scene>> scenes;
};

} // namespace bembel
#endif // include guards
