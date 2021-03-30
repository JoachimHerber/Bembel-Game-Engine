#include "./input-device.hpp"

#include "./input-device-button.hpp"

namespace bembel::kernel {

InputDevice::InputDevice(EventManager& event_mgr, const std::string& name)
: event_mgr(event_mgr)
, name(name) {
}

InputDevice::~InputDevice() {
}

const std::string& InputDevice::getDeviceName() const {
    return this->name;
}

InputDeviceButton* InputDevice::getButton(const std::string& name) const {
    for(auto& it : this->buttons) {
        if(it->getName() == name) return it.get();
    }
    return nullptr;
}

} // namespace bembel::kernel
