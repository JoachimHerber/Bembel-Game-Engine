#ifndef BEMBEL_INPUTEXAMPLE_HPP
#define BEMBEL_INPUTEXAMPLE_HPP

#include <bembel/bembel.hpp>

namespace bembel {

class GUIExample : public kernel::Application {
  public:
    GUIExample();
    ~GUIExample();

    virtual bool init() override;
    virtual void cleanup() override;

    virtual void update(double time) override;

    void handleEvent(const kernel::WindowShouldCloseEvent&);

    void onButton1Click();
    void onButton2Click();
    void onButton3Click();

  private:
    gui::GUISystem* gui_system;
};

} // namespace bembel
#endif // include guards
