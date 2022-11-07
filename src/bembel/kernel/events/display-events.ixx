export module bembel.kernel : DisplayEvents;

import bembel.base;

namespace bembel::kernel {
using namespace base;

class Window;

export struct WindowOpendEvent {
    Window* const window;
};
export struct WindowClosedEvent {
    Window* const window;
};
export struct WindowUpdateEvent {
    Window* const window;
};

export struct WindowMovedEvent {
    Window* const   window;
    const glm::vec2 position;
};
export struct WindowResizeEvent {
    Window* const   window;
    const glm::vec2 size;
};

export struct WindowShouldCloseEvent {
    Window* const window;
};

export struct WindowGainedFocusEvent {
    Window* const window;
};
export struct WindowLostFocusEvent {
    Window* const window;
};

export struct WindowIconifyedEvent {
    Window* const window;
};
export struct WindowRestoredEvent {
    Window* const window;
};

export struct FrameBufferResizeEvent {
    Window* const   window;
    const glm::vec2 size;
};

} // namespace bembel::kernel
