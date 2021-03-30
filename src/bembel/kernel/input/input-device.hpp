#ifndef BEMBEL_INPUT_DEVICES_INPUTDEVICE_HPP
#define BEMBEL_INPUT_DEVICES_INPUTDEVICE_HPP

#include <bembel/kernel/events/event-manager.hpp>

#include "./input-device-button.hpp"

namespace bembel::kernel {

class InputDeviceButton;

class BEMBEL_API InputDevice {
  protected:
    InputDevice(EventManager& eventMgr, const std::string& name);
    InputDevice(const InputDevice& other) = delete;
    virtual ~InputDevice();

  public:
    const std::string& getDeviceName() const;

    InputDeviceButton* getButton(const std::string& name) const;

  protected:
    EventManager&     event_mgr;
    const std::string name;

    std::vector<std::unique_ptr<InputDeviceButton>> buttons;
};

} // namespace bembel::kernel
#endif // include guards
