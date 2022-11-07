module;
#include <string>
export module bembel.examples.rendering;

import bembel;
import : CameraControle;

namespace bembel {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;

export class RenderingExample : public kernel::Application {
  public:
    RenderingExample();
    ~RenderingExample();

    virtual bool init() override;
    virtual void cleanup() override;

    virtual void update(double time) override;

    void         handleEvent(const kernel::WindowShouldCloseEvent&);

  private:
    GraphicSystem*                              m_graphic_system;
    GuiSystem*                                  m_gui_system;
    LabelWidget*                                m_label;

    std::unique_ptr<CameraControle>             m_camera;

    std::vector<std::shared_ptr<kernel::Scene>> m_scenes;
};

} // namespace bembel
