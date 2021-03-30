#ifndef BEMBEL_INPUT_DEVICES_MOUSE_HPP
#define BEMBEL_INPUT_DEVICES_MOUSE_HPP

#include <bembel/kernel/events/event-manager.hpp>
#include <bembel/kernel/events/input-events.hpp>

#include "./input-device.hpp"

namespace bembel::kernel {

class BEMBEL_API Mouse : public InputDevice {
  public:
    Mouse(EventManager&);
    Mouse(const Mouse&) = delete;
    Mouse& operator=(const Mouse&) = delete;
    ~Mouse();

    InputDeviceButton* getButton(int);

    void handleEvent(const MouseButtonPressEvent&);
    void handleEvent(const MouseButtonReleaseEvent&);
};

} // namespace bembel::kernel
#endif // include guards
