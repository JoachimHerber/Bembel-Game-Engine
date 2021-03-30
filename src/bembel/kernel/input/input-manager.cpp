#include "./input-manager.hpp"

#include <bembel/base/io/xml.hpp>
#include <bembel/base/logging/logger.hpp>
#include <bembel/kernel/core/kernel.hpp>

namespace bembel::kernel {

InputManager::InputManager(EventManager& event_mgr)
: mouse{event_mgr}
, keyboard{event_mgr} {
}
InputManager::~InputManager() {
}

Mouse& InputManager::getMouse() {
    return this->mouse;
}

Keyboard& InputManager::getKeyboard() {
    return this->keyboard;
}

} // namespace bembel::kernel
