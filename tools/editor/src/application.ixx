module;
#include <string_view>
export module bembel.tools.editor;

import bembel;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::graphics;
using namespace bembel::gui;
using namespace bembel::physics;

export class Application : public kernel::Application {
  public:
    Application();
    ~Application();

    virtual bool init() override;
    virtual void cleanup() override;

    virtual void update(double time) override;

    void handleEvent(In<WindowShouldCloseEvent>);
    void handleEvent(In<FrameBufferResizeEvent>);

  private:
    bool initUserInterface();

  private:
    GraphicSystem* m_graphic_system;
    PhysicsSystem* m_physics_system;
    GuiSystem*     m_gui_system;

    Window*                 m_main_window;
    GraphicalUserInterface* m_gui;
    TabGroupWidget*         m_tabs;

    std::unique_ptr<ModelManager> m_mgr;
};

} // namespace bembel::tools
