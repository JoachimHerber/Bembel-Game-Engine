module;
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <string_view>

export module bembel.tools.editor;

import bembel;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::graphics;
using namespace bembel::gui;
using namespace bembel::physics;

export class Application
  : public kernel::Application<GraphicSystem, PhysicsSystem, GuiSystem>
  , public imgui::View {
  public:
    Application();
    ~Application();

    virtual bool init(std::span<std::string_view> args) override;
    virtual void cleanup() override;

    virtual void update(double time) override;

    void handleEvent(In<WindowShouldCloseEvent>);
    void handleEvent(In<FrameBufferResizeEvent>);
    void handleEvent(In<FileDropEvent>);

  private:
    virtual void drawUI(In<ivec2> viewport_position, In<uvec2> viewport_size) override;

    void initScene();

  private:
    EventHandlerGuard<WindowShouldCloseEvent, FrameBufferResizeEvent, FileDropEvent> m_guard{this};

    Window*                 m_main_window;
    GraphicalUserInterface* m_gui;

    RenderingPipeline*     m_pipeline;

    float m_camera_pitch = 0;
    float m_camera_yaw   = 0;
    float m_camera_dist  = 2.5f;

    Assimp::Importer m_importer;
};

} // namespace bembel::tools
