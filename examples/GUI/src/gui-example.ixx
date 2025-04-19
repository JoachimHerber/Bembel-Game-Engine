export module bembel.examples.gui;

import std;
import bembel;

namespace bembel {

export class GuiExample : public kernel::Application<gui::GuiSystem> {
  public:
    GuiExample();
    ~GuiExample();

    virtual bool init(std::span<std::string_view> args) override;
    virtual void cleanup() override;

    void handleEvent(const kernel::WindowShouldCloseEvent&);

    void onButton1Click();
    void onButton2Click();
    void onButton3Click();

  private:
};

} // namespace bembel
