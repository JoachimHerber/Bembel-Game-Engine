export module bembel.examples.gui;

import bembel;

namespace bembel {

export class GuiExample : public kernel::Application {
  public:
    GuiExample();
    ~GuiExample();

    virtual bool init() override;
    virtual void cleanup() override;

    virtual void update(double time) override;

    void         handleEvent(const kernel::WindowShouldCloseEvent&);

    void         onButton1Click();
    void         onButton2Click();
    void         onButton3Click();

  private:
    gui::GuiSystem* m_gui_system;
};

} // namespace bembel
