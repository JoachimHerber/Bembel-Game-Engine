module;
#include <GLFW/glfw3.h>

#include "bembel/pch.h"
module bembel.kernel.input;

import bembel.base;

namespace bembel::kernel {
using namespace bembel::base;

Mouse::Mouse(EventManager& eventMgr) : InputDevice(eventMgr, "Mouse") {
    m_event_mgr.addHandler<MouseButtonPressEvent>(this);
    m_event_mgr.addHandler<MouseButtonReleaseEvent>(this);
}

Mouse::~Mouse() {
    m_event_mgr.removeHandler<MouseButtonPressEvent>(this);
    m_event_mgr.removeHandler<MouseButtonReleaseEvent>(this);
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
        m_event_mgr.broadcast(InputDeviceButtonPressEvent{button});
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
        m_event_mgr.broadcast(InputDeviceButtonReleaseEvent{button});
        button->setIsPressed(false);
    } else {
        log().error(
            "Received MouseButtonReleaseEvent for unsupported Mouse button {}", event.button_id
        );
    }
}

} // namespace bembel::kernel
