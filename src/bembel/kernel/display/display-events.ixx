module;
#include <filesystem>
#include <span>
export module bembel.kernel.display:Events;

import bembel.base;

import :Window;

namespace bembel::kernel {
using namespace base;

export struct WindowOpendEvent final {
    const not_null_ptr<Window> window;
};
export struct WindowClosedEvent final {
    const not_null_ptr<Window> window;
};
export struct WindowUpdateEvent final {
    const not_null_ptr<Window> window;
};

export struct WindowMovedEvent final {
    const not_null_ptr<Window> window;
    const ivec2                position;
};
export struct WindowResizeEvent final {
    const not_null_ptr<Window> window;
    const ivec2                size;
};

export struct WindowShouldCloseEvent final {
    const not_null_ptr<Window> window;
};

export struct WindowGainedFocusEvent final {
    const not_null_ptr<Window> window;
};
export struct WindowLostFocusEvent final {
    const not_null_ptr<Window> window;
};

export struct WindowIconifyedEvent final {
    const not_null_ptr<Window> window;
};
export struct WindowRestoredEvent final {
    const not_null_ptr<Window> window;
};

export struct FrameBufferResizeEvent final {
    const not_null_ptr<Window> window;
    const ivec2                size;
};

export struct KeyPressEvent final {
    const not_null_ptr<Window> window;
    const int                  key_id;
    const int                  scancode;
    const int                  mods;
};

export struct KeyRepeatEvent final {
    const not_null_ptr<Window> window;
    const int                  key_id;
    const int                  scancode;
    const int                  mods;
};

export struct KeyReleaseEvent final {
    const not_null_ptr<Window> window;
    const int                  key_id;
    const int                  scancode;
    const int                  mods;
};

export struct TextInputEvent final {
    const not_null_ptr<Window> window;
    const char32_t             character;
};

export struct MouseButtonPressEvent final {
    const not_null_ptr<Window> window;
    const int                  button_id;
    const int                  mods;
};
export struct MouseButtonRepeatEvent final {
    const not_null_ptr<Window> window;
    const int                  button_id;
    const int                  mods;
};
export struct MouseButtonReleaseEvent final {
    const not_null_ptr<Window> window;
    const int                  button_id;
    const int                  mods;
};

export struct CursorMovedEvent final {
    const not_null_ptr<Window> window;
    const dvec2                position;
};

export struct CursorEnteredEvent final {
    const not_null_ptr<Window> window;
};
export struct CursorLeftEvent final {
    const not_null_ptr<Window> window;
};

export struct ScrollEvent final {
    const not_null_ptr<Window> window;
    const double               x;
    const double               y;
};

export struct FileDropEvent final {
    const not_null_ptr<Window>               window;
    const std::vector<std::filesystem::path> files;
};

} // namespace bembel::kernel
