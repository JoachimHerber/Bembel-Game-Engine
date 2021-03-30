#include "./input-device-button.hpp"

namespace bembel::kernel {

InputDeviceButton::InputDeviceButton(InputDevice* device, const std::string& name)
: device(device)
, name(name)
, pressed(false) {
}

InputDeviceButton::~InputDeviceButton() {
}

InputDevice* InputDeviceButton::getDevice() const {
    return this->device;
}

const std::string& InputDeviceButton::getName() const {
    return this->name;
}

bool InputDeviceButton::getIsPressed() const {
    return this->pressed;
}

void InputDeviceButton::setIsPressed(bool b) {
    if(this->pressed == b) return;

    this->pressed = b;
    if(this->pressed)
        this->press_signal.emit();
    else
        this->release_signal.emit();
}

} // namespace bembel::kernel
