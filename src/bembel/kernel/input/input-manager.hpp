#ifndef BEMBEL_INPUT_INPUTSYSTEM_HPP
#define BEMBEL_INPUT_INPUTSYSTEM_HPP

#include <bembel/library.hpp>

#include "./keyboard.hpp"
#include "./mouse.hpp"

namespace bembel::kernel {

class EventManager;

class BEMBEL_API InputManager {
  public:
    InputManager(EventManager& event_mgr);
    ~InputManager();

    Mouse&    getMouse();
    Keyboard& getKeyboard();

  private:
    Mouse    mouse;
    Keyboard keyboard;
};

} // namespace bembel::kernel
#endif // include guards
