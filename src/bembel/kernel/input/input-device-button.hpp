#ifndef BEMBEL_INPUT_BUTTON_HPP
#define BEMBEL_INPUT_BUTTON_HPP

#include <bembel/base/utils/signal.hpp>
#include <bembel/kernel/events/event-manager.hpp>

namespace bembel::kernel {

class InputDevice;

class BEMBEL_API InputDeviceButton {
 public:
  InputDeviceButton(InputDevice* device, const std::string& name);
  ~InputDeviceButton();

  InputDevice* getDevice() const;
  const std::string& getName() const;

  bool getIsPressed() const;
  void setIsPressed(bool);

  base::Signal<> press_signal;
  base::Signal<> release_signal;

 protected:
  InputDevice* device;
  std::string name;

  bool pressed;
};

struct InputDeviceButtonPressEvent {
  // the button that was pressed
  InputDeviceButton* button;
};

struct InputDeviceButtonReleaseEvent {
  // the button that has been released
  InputDeviceButton* button;
};

} // namespace bembel::kernel
#endif // include guards
