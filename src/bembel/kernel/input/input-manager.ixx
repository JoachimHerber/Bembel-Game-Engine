export module bembel.kernel.input:InputManager;

import :Mouse;
import :Keyboard;

namespace bembel::kernel {

export class InputManager {
  public:
    InputManager()  = default;
    ~InputManager() = default;

    Mouse    mouse;
    Keyboard keyboard;
};

} // namespace bembel::kernel
