module;
#include <filesystem>
export module bembel.examples.gui;

import bembel;

namespace bembel {

export class GuiExample : public kernel::Application<gui::GuiSystem> {
  public:
    GuiExample();
    ~GuiExample();

    virtual bool init(std::span<std::string_view> args) override;
    virtual void cleanup() override;

    virtual void update(double time) override;

    void         handleEvent(const kernel::WindowShouldCloseEvent&);

    void         onButton1Click();
    void         onButton2Click();
    void         onButton3Click();

  private:
};

} // namespace bembel
