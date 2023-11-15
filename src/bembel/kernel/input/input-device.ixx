module;
#include <string_view>
export module bembel.kernel.input:InputDevice;

import bembel.base;

namespace bembel::kernel {
using namespace bembel::base;

export class InputDevice {
  public:
    class Button {
      public:
        Button(InputDevice& device, std::string_view name) : m_device{device}, m_name{name} {}
        ~Button() = default;

        InputDevice&       getDevice() const { return m_device; }
        std::string const& getName() const { return m_name; }

        bool getIsPressed() const { return m_pressed; }
        void setIsPressed(bool pressed) {
            if(m_pressed == pressed) return;

            m_pressed = pressed;
            if(m_pressed)
                press_signal.emit();
            else
                release_signal.emit();
        }

        Signal<> press_signal;
        Signal<> release_signal;

      protected:
        InputDevice& m_device;
        std::string  m_name;

        bool m_pressed = false;
    };

    virtual Button* getButton(std::string_view name) = 0;

    std::string const& getDeviceName() const { return m_name; }

  protected:
    InputDevice(std::string_view name) : m_name{name} {}
    InputDevice(InputDevice const& other) = delete;
    virtual ~InputDevice()                = default;

  protected:
    std::string const m_name;
};

export struct InputDeviceButtonPressEvent {
    // the button that was pressed
    InputDevice::Button* button;
};

export struct InputDeviceButtonReleaseEvent {
    // the button that has been released
    InputDevice::Button* button;
};

} // namespace bembel::kernel
