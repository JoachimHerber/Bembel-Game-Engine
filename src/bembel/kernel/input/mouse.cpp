module;
#include <string_view>
module bembel.kernel.input;

import bembel.base;

namespace bembel::kernel {
using namespace bembel::base;

Mouse::Mouse() : InputDevice("Mouse") {}

Mouse::~Mouse() {}

InputDevice::Button* Mouse::getButton(In<u64> button_id) {
    if(button_id >= NUM_BUTTONS) {
        logError("Mouse button {} not supported", button_id);
        return nullptr;
    }

    return &m_buttons[button_id];
}

Mouse::Button* Mouse::getButton(In<std::string_view> name) {
    for(auto& button : m_buttons) {
        if(button.getName() == name) return &button;
    }
    return nullptr;
}

void Mouse::handleEvent(In<MouseButtonPressEvent> event) {
    if(event.button_id < NUM_BUTTONS) {
        Button* button = &m_buttons[event.button_id];
        events::broadcast<InputDeviceButtonPressEvent>(button);
        button->setIsPressed(true);
    } else {
        logError("Received MouseButtonPressEvent for unsupported Mouse button {}", event.button_id);
    }
}

void Mouse::handleEvent(In<MouseButtonReleaseEvent> event) {
    if(event.button_id < NUM_BUTTONS) {
        Button* button = &m_buttons[event.button_id];
        events::broadcast<InputDeviceButtonReleaseEvent>(button);
        button->setIsPressed(false);
    } else {
        logError(
            "Received MouseButtonReleaseEvent for unsupported Mouse button {}", event.button_id
        );
    }
}

} // namespace bembel::kernel
