module;
#include <filesystem>
#include <span>
export module bembel.kernel.display:Events;

import bembel.base;

import :Window;

namespace bembel::kernel {
using namespace base;

export struct WindowOpendEvent final {
    not_null_ptr<Window> window;
};
export struct WindowClosedEvent final {
    not_null_ptr<Window> window;
};
export struct WindowUpdateEvent final {
    not_null_ptr<Window> window;
};

export struct WindowMovedEvent final {
    not_null_ptr<Window> window;
    ivec2                position;
};
export struct WindowResizeEvent final {
    not_null_ptr<Window> window;
    ivec2                size;
};

export struct WindowShouldCloseEvent final {
    not_null_ptr<Window> window;
};

export struct WindowGainedFocusEvent final {
    not_null_ptr<Window> window;
};
export struct WindowLostFocusEvent final {
    not_null_ptr<Window> window;
};

export struct WindowIconifyedEvent final {
    not_null_ptr<Window> window;
};
export struct WindowRestoredEvent final {
    not_null_ptr<Window> window;
};

export struct FrameBufferResizeEvent final {
    not_null_ptr<Window> window;
    ivec2                size;
};

export struct KeyPressEvent final {
    not_null_ptr<Window> window;
    int                  key_id;
    int                  scancode;
    int                  mods;
};

export struct KeyRepeatEvent final {
    not_null_ptr<Window> window;
    int                  key_id;
    int                  scancode;
    int                  mods;
};

export struct KeyReleaseEvent final {
    not_null_ptr<Window> window;
    int                  key_id;
    int                  scancode;
    int                  mods;
};

export struct TextInputEvent final {
    not_null_ptr<Window> window;
    char32_t             character;
};

export struct MouseButtonPressEvent final {
    not_null_ptr<Window> window;
    int                  button_id;
    int                  mods;
};
export struct MouseButtonRepeatEvent final {
    not_null_ptr<Window> window;
    int                  button_id;
    int                  mods;
};
export struct MouseButtonReleaseEvent final {
    not_null_ptr<Window> window;
    int                  button_id;
    int                  mods;
};

export struct CursorMovedEvent final {
    not_null_ptr<Window> window;
    dvec2                position;
};

export struct CursorEnteredEvent final {
    not_null_ptr<Window> window;
};
export struct CursorLeftEvent final {
    not_null_ptr<Window> window;
};

export struct ScrollEvent final {
    not_null_ptr<Window> window;
    double               x;
    double               y;
};

export struct FileDropEvent final {
    not_null_ptr<Window>               window;
    std::vector<std::filesystem::path> files;
};

} // namespace bembel::kernel
