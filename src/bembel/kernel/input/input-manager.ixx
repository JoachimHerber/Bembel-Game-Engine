export module bembel.kernel.input:InputManager;

import bembel.base;
import :Mouse;
import :Keyboard;

namespace bembel::kernel {
using namespace bembel::base;

export class InputManager {
  public:
    InputManager()  = default;
    ~InputManager() = default;

    Mouse    mouse;
    Keyboard keyboard;
};

} // namespace bembel::kernel
