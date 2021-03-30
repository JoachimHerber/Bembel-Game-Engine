#include "./mouse.hpp"

#include <GLFW/glfw3.h>

#include <bembel/base/logging/logger.hpp>
#include <bembel/kernel/events/event-manager.hpp>

namespace bembel::kernel {

Mouse::Mouse(kernel::EventManager& eventMgr)
: InputDevice(eventMgr, "Mouse") {
    this->buttons.push_back(std::make_unique<InputDeviceButton>(this, "Mouse L"));
    this->buttons.push_back(std::make_unique<InputDeviceButton>(this, "Mouse R"));
    this->buttons.push_back(std::make_unique<InputDeviceButton>(this, "Mouse M"));
    this->buttons.push_back(std::make_unique<InputDeviceButton>(this, "Mouse 4"));
    this->buttons.push_back(std::make_unique<InputDeviceButton>(this, "Mouse 5"));
    this->buttons.push_back(std::make_unique<InputDeviceButton>(this, "Mouse 6"));
    this->buttons.push_back(std::make_unique<InputDeviceButton>(this, "Mouse 7"));
    this->buttons.push_back(std::make_unique<InputDeviceButton>(this, "Mouse 8"));
    this->buttons.push_back(std::make_unique<InputDeviceButton>(this, "Mouse 9"));
    this->buttons.push_back(std::make_unique<InputDeviceButton>(this, "Mouse 10"));
    this->buttons.push_back(std::make_unique<InputDeviceButton>(this, "Mouse 11"));
    this->buttons.push_back(std::make_unique<InputDeviceButton>(this, "Mouse 12"));
    this->buttons.push_back(std::make_unique<InputDeviceButton>(this, "Mouse 13"));
    this->buttons.push_back(std::make_unique<InputDeviceButton>(this, "Mouse 14"));
    this->buttons.push_back(std::make_unique<InputDeviceButton>(this, "Mouse 15"));
    this->buttons.push_back(std::make_unique<InputDeviceButton>(this, "Mouse 16"));

    this->event_mgr.addHandler<MouseButtonPressEvent>(this);
    this->event_mgr.addHandler<MouseButtonReleaseEvent>(this);
}

Mouse::~Mouse() {
    this->event_mgr.removeHandler<MouseButtonPressEvent>(this);
    this->event_mgr.removeHandler<MouseButtonReleaseEvent>(this);
}

InputDeviceButton* Mouse::getButton(int button_id) {
    if(button_id < 0 || static_cast<size_t>(button_id) >= this->buttons.size()) {
        BEMBEL_LOG_ERROR() << "Mouse button " << button_id << " not supported";
        return nullptr;
    }

    return this->buttons[button_id].get();
}

void Mouse::handleEvent(const kernel::MouseButtonPressEvent& event) {
    auto button = this->getButton(event.button_id);
    this->event_mgr.broadcast(InputDeviceButtonPressEvent{button});
    button->setIsPressed(true);
}

void Mouse::handleEvent(const kernel::MouseButtonReleaseEvent& event) {
    auto button = getButton(event.button_id);
    this->event_mgr.broadcast(InputDeviceButtonReleaseEvent{button});
    button->setIsPressed(false);
}

} // namespace bembel::kernel
