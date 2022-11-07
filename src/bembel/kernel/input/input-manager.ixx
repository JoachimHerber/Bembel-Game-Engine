export module bembel.kernel.input : InputManager;

import bembel.base;
import : Mouse;
import : Keyboard;

namespace bembel::kernel {
using namespace bembel::base;

export class InputManager {
  public:
    InputManager(EventManager& event_mgr) : mouse{event_mgr}, keyboard{event_mgr} {}
    ~InputManager() = default;

    Mouse&    getMouse() { return this->mouse; }
    Keyboard& getKeyboard() { return this->keyboard; }

  private:
    Mouse    mouse;
    Keyboard keyboard;
};

} // namespace bembel::kernel
