#ifndef BEMBEL_KERNEL_EVENTS_DISPLAYEVENTS_HPP
#define BEMBEL_KERNEL_EVENTS_DISPLAYEVENTS_HPP

#include "event-manager.hpp"

namespace bembel::kernel {

class Window;

struct WindowOpendEvent {
    Window* const window;
};
struct WindowClosedEvent {
    Window* const window;
};
struct WindowUpdateEvent {
    Window* const window;
};

struct WindowMovedEvent {
    Window* const window;
    const glm::vec2 position;
};
struct WindowResizeEvent {
    Window* const window;
    const glm::vec2 size;
};

struct WindowShouldCloseEvent {
    Window* const window;
};

struct WindowGainedFocusEvent {
    Window* const window;
};
struct WindowLostFocusEvent {
    Window* const window;
};

struct WindowIconifyedEvent {
    Window* const window;
};
struct WindowRestoredEvent {
    Window* const window;
};

struct FrameBufferResizeEvent {
    Window* const window;
    const glm::vec2 size;
};

} // namespace bembel::kernel
#endif // include guards
