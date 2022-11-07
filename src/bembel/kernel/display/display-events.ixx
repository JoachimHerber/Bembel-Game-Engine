module;
#include "bembel/pch.h"
export module bembel.kernel.display:Events;

import bembel.base;

import :Window;

namespace bembel::kernel {
using namespace base;

export struct WindowOpendEvent final {
    Window* const window;
};
export struct WindowClosedEvent final {
    Window* const window;
};
export struct WindowUpdateEvent final {
    Window* const window;
};

export struct WindowMovedEvent final {
    Window* const window;
    vec2 const    position;
};
export struct WindowResizeEvent final {
    Window* const window;
    vec2 const    size;
};

export struct WindowShouldCloseEvent final {
    Window* const window;
};

export struct WindowGainedFocusEvent final {
    Window* const window;
};
export struct WindowLostFocusEvent final {
    Window* const window;
};

export struct WindowIconifyedEvent final {
    Window* const window;
};
export struct WindowRestoredEvent final {
    Window* const window;
};

export struct FrameBufferResizeEvent final {
    Window* const window;
    vec2 const    size;
};

export struct KeyPressEvent final {
    Window* const window;
    int const     key_id;
    int const     scancode;
    int const     mods;
};

export struct KeyRepeatEvent final {
    Window* const window;
    int const     key_id;
    int const     scancode;
    int const     mods;
};

export struct KeyReleaseEvent final {
    Window* const window;
    int const     key_id;
    int const     scancode;
    int const     mods;
};

export struct TextInputEvent final {
    Window* const  window;
    char32_t const character;
};

export struct MouseButtonPressEvent final {
    Window* const window;
    int const     button_id;
    int const     mods;
};
export struct MouseButtonRepeatEvent final {
    Window* const window;
    int const     button_id;
    int const     mods;
};
export struct MouseButtonReleaseEvent final {
    Window* const window;
    int const     button_id;
    int const     mods;
};

export struct CursorMovedEvent final {
    Window* const window;
    dvec2 const   position;
};

export struct CursorEnteredEvent final {
    Window* const window;
};
export struct CursorLeftEvent final {
    Window* const window;
};

export struct ScrollEvent final {
    Window* const window;
    double const  x;
    double const  y;
};

export struct FileDropEvent final {
    Window* const                  window;
    std::vector<std::string> const files;
};

} // namespace bembel::kernel
