module;
#include <string_view>
export module bembel.kernel.input:Mouse;

import bembel.base;
import bembel.kernel.display;

import :InputDevice;

namespace bembel::kernel {
using namespace bembel::base;

export class Mouse : public InputDevice {
  public:
    Mouse();
    Mouse(Mouse const&)            = delete;
    Mouse& operator=(Mouse const&) = delete;
    ~Mouse();

    Button* getButton(In<u64>);
    Button* getButton(In<std::string_view> name) override;

    void handleEvent(In<MouseButtonPressEvent>);
    void handleEvent(In<MouseButtonReleaseEvent>);

  private:
    static constexpr usize NUM_BUTTONS = 16;

    EventHandlerGuard<MouseButtonPressEvent, MouseButtonReleaseEvent> m_guard{this};

    Button m_buttons[NUM_BUTTONS]{
        {this, "Mouse L"},
        {this, "Mouse R"},
        {this, "Mouse M"},
        {this, "Mouse 4"},
        {this, "Mouse 5"},
        {this, "Mouse 6"},
        {this, "Mouse 7"},
        {this, "Mouse 8"},
        {this, "Mouse 9"},
        {this, "Mouse 10"},
        {this, "Mouse 11"},
        {this, "Mouse 12"},
        {this, "Mouse 13"},
        {this, "Mouse 14"},
        {this, "Mouse 15"},
        {this, "Mouse 16"}};
};

} // namespace bembel::kernel
