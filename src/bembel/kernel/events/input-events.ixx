export module bembel.kernel : InputEvents;

import<vector>;
import<string>;

import bembel.base;

namespace bembel::kernel {
using namespace base;

class Window;

export struct KeyPressEvent {
    Window* const window;
    int           key_id;
    int           scancode;
    int           mods;
};

export struct KeyRepeatEvent {
    Window* const window;
    int           key_id;
    int           scancode;
    int           mods;
};

export struct KeyReleaseEvent {
    Window* const window;
    int           key_id;
    int           scancode;
    int           mods;
};

export struct TextInputEvent {
    Window* const window;
    char32_t      character;
};

export struct MouseButtonPressEvent {
    Window* const window;
    int           button_id;
    int           mods;
};
export struct MouseButtonRepeatEvent {
    Window* const window;
    int           button_id;
    int           mods;
};
export struct MouseButtonReleaseEvent {
    Window* const window;
    int           button_id;
    int           mods;
};

export struct CursorMovedEvent {
    Window* const window;
    glm::dvec2    position;
};

export struct CursorEnteredEvent {
    Window* const window;
};
export struct CursorLeftEvent {
    Window* const window;
};

export struct ScrollEvent {
    Window* const window;
    double        x;
    double        y;
};

export struct FileDropEvent {
    Window*                  window;
    std::vector<std::string> files;
};

} // namespace bembel::kernel
