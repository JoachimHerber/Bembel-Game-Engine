module;
#include <string>
export module bembel.examples.rendering;

import bembel;
import :CameraControle;

namespace bembel {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;

export class RenderingExample : public kernel::Application<GraphicSystem, GuiSystem> {
  public:
    RenderingExample();
    ~RenderingExample();

    virtual bool init(std::span<std::string_view> args) override;
    virtual void cleanup() override;

    virtual void update(double time) override;

    void handleEvent(In<kernel::WindowShouldCloseEvent>);
    void handleEvent(In<kernel::FrameBufferResizeEvent>);

  private:
    void updateLightDir(In<i64>);

  private:
    class ShadowDebugView : public Viewport::View {
      public:
        ShadowDebugView(Texture* texture, u64 resolution, uint layer);
        ~ShadowDebugView() = default;

        void draw(ivec2 const& viewport_position, uvec2 const& viewport_size) override;

      private:
        std::unique_ptr<ShaderProgram> m_shader;
        Texture*                       m_texture;
        u64                            m_resolution;
        uint                           m_layer;
    };
    LabelWidget*     m_label;
    IntSliderWidget* m_light_slider_pitch;
    IntSliderWidget* m_light_slider_yaw;

    std::array<std::unique_ptr<ShadowDebugView>, 3> m_views;

    std::unique_ptr<CameraControle> m_camera;

    std::shared_ptr<kernel::Scene> m_scene;

    Entity<> m_light;
};

} // namespace bembel
