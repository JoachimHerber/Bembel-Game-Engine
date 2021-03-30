#ifndef BEMBEL_INPUT_DEVICES_KEYBOARD_HPP
#define BEMBEL_INPUT_DEVICES_KEYBOARD_HPP

#include <bembel/kernel/events/event-manager.hpp>
#include <bembel/kernel/events/input-events.hpp>

#include "./input-device.hpp"

namespace bembel::kernel {

class BEMBEL_API Keyboard : public InputDevice {
  public:
    Keyboard(kernel::EventManager&);
    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard&) = delete;
    ~Keyboard();

    InputDeviceButton* getKey(int key_id, int scancode = -1);

    void handleEvent(const kernel::KeyPressEvent&);
    void handleEvent(const kernel::KeyReleaseEvent&);

  private:
    void initKey(int key_id, const std::string& name);

  private:
    std::map<std::pair<int, int>, unsigned> key_mapping;
};

} // namespace bembel::kernel
#endif // include guards
