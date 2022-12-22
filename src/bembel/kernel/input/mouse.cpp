module;
#include <string_view>
module bembel.kernel.input;

import bembel.base;

namespace bembel::kernel {
using namespace bembel::base;

Mouse::Mouse() : InputDevice("Mouse") {
    events::addHandler<MouseButtonPressEvent>(this);
    events::addHandler<MouseButtonReleaseEvent>(this);
}

Mouse::~Mouse() {
    events::removeHandler<MouseButtonPressEvent>(this);
    events::removeHandler<MouseButtonReleaseEvent>(this);
}

InputDevice::Button* Mouse::getButton(u64 button_id) {
    if(button_id >= NUM_BUTTONS) {
        log().error("Mouse button {} not supported", button_id);
        return nullptr;
    }

    return &m_buttons[button_id];
}

Mouse::Button* Mouse::getButton(std::string_view name) {
    for(auto& button : m_buttons) {
        if(button.getName() == name) return &button;
    }
    return nullptr;
}

void Mouse::handleEvent(MouseButtonPressEvent const& event) {
    if(event.button_id < NUM_BUTTONS) {
        Button* button = &m_buttons[event.button_id];
        events::broadcast<InputDeviceButtonPressEvent>(button);
        button->setIsPressed(true);
    } else {
        log().error(
            "Received MouseButtonPressEvent for unsupported Mouse button {}", event.button_id
        );
    }
}

void Mouse::handleEvent(MouseButtonReleaseEvent const& event) {
    if(event.button_id < NUM_BUTTONS) {
        Button* button = &m_buttons[event.button_id];
        events::broadcast<InputDeviceButtonReleaseEvent>(button);
        button->setIsPressed(false);
    } else {
        log().error(
            "Received MouseButtonReleaseEvent for unsupported Mouse button {}", event.button_id
        );
    }
}

} // namespace bembel::kernel
