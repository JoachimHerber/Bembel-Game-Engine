#ifndef BEMBEL_KERNEL_EVENTS_INPUTEVENTS_HPP
#define BEMBEL_KERNEL_EVENTS_INPUTEVENTS_HPP

#include "./event-manager.hpp"

namespace bembel::kernel {

class Window;

struct KeyPressEvent {
    Window* const window;
    int key_id;
    int scancode;
    int mods;
};

struct KeyRepeatEvent {
    Window* const window;
    int key_id;
    int scancode;
    int mods;
};

struct KeyReleaseEvent {
    Window* const window;
    int key_id;
    int scancode;
    int mods;
};

struct TextInputEvent {
    Window* const window;
    char32_t character;
};

struct MouseButtonPressEvent {
    Window* const window;
    int button_id;
    int mods;
};
struct MouseButtonRepeatEvent {
    Window* const window;
    int button_id;
    int mods;
};
struct MouseButtonReleaseEvent {
    Window* const window;
    int button_id;
    int mods;
};

struct CursorMovedEvent {
    Window* const window;
    glm::dvec2 position;
};

struct CursorEnteredEvent {
    Window* const window;
};
struct CursorLeftEvent {
    Window* const window;
};

struct ScrollEvent {
    Window* const window;
    double x;
    double y;
};

struct FileDropEvent {
    Window* window;
    std::vector<std::string> files;
};

} // namespace bembel::kernel
#endif // include guards
