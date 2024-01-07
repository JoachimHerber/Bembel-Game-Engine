module;
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <string_view>
export module bembel.tools.editor;

import bembel;
import :MaterialMgr;
import :ModelMgr;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::graphics;
using namespace bembel::gui;
using namespace bembel::physics;

export class Application : public kernel::Application<GraphicSystem, PhysicsSystem, GuiSystem> {
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
    bool initUserInterface(Viewport& preview);
    bool initRenderingPipeline(Viewport& preview);

  private:
    EventHandlerGuard<WindowShouldCloseEvent, FrameBufferResizeEvent, FileDropEvent> m_guard{this};

    Window*                 m_main_window;
    GraphicalUserInterface* m_gui;
    SplitGroupWidget*       m_root;
    TabGroupWidget*         m_tabs;

    RenderingPipeline*            m_pipeline;

    std::unique_ptr<MaterialManager> m_material_mgr;
    std::unique_ptr<ModelManager>    m_model_mgr;

    std::vector<std::pair<std::unique_ptr<Assimp::Importer>, aiScene const*>> m_imports;
};

} // namespace bembel::tools
