export module bembel.gui:System;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class GuiSystem : public System {
  public:
    GuiSystem(Engine&);
    GuiSystem(GuiSystem const&)            = delete;
    GuiSystem& operator=(GuiSystem const&) = delete;
    ~GuiSystem();

    virtual bool configure(xml::Element const*) override;

    virtual bool init() override;
    virtual void shutdown() override;
    virtual void update(double time_since_last_update) override;

    GraphicalUserInterface* createGUI(std::string_view name);

    GraphicalUserInterface* getGUI(std::string_view name);

  private:
    Engine&                                              m_engine;
    std::vector<std::unique_ptr<GraphicalUserInterface>> m_guis;
    Dictionary<GraphicalUserInterface*>                  m_named_guis;
};

} // namespace bembel::gui
