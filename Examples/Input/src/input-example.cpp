module;
#include <string>
module bembel.examples.input;

namespace bembel {
using namespace bembel::base;
using namespace bembel::kernel;

InputExample::InputExample() : Application() {
    events::addHandler<WindowShouldCloseEvent>(this);

    events::addHandler<KeyPressEvent>(this);
    events::addHandler<KeyRepeatEvent>(this);
    events::addHandler<KeyReleaseEvent>(this);
    events::addHandler<TextInputEvent>(this);
    events::addHandler<MouseButtonPressEvent>(this);
    events::addHandler<MouseButtonRepeatEvent>(this);
    events::addHandler<MouseButtonReleaseEvent>(this);
    events::addHandler<CursorMovedEvent>(this);
    events::addHandler<CursorEnteredEvent>(this);
    events::addHandler<CursorLeftEvent>(this);
    events::addHandler<ScrollEvent>(this);

    events::addHandler<InputDeviceButtonPressEvent>(this);
    events::addHandler<InputDeviceButtonReleaseEvent>(this);
}

InputExample::~InputExample() {}

bool InputExample::init() {
    auto window = m_engine.display.createWindow();
    window->open("Input Example");
    m_engine.initSystems();

    auto& keyboard = m_engine.input.keyboard;

    keyboard.getKey(Keyboard::LEFT)->release_signal.bind(this, &InputExample::pervCursor);
    keyboard.getKey(Keyboard::RIGHT)->release_signal.bind(this, &InputExample::nextCursor);

    m_cursor.resize(6);
    m_cursor[0].request("Arrow");
    m_cursor[1].request("IBeam");
    m_cursor[2].request("Crosshair");
    m_cursor[3].request("Hand");
    m_cursor[4].request("HResize");
    m_cursor[5].request("VResize");
    return true;
}

void InputExample::cleanup() {
    m_engine.shutdownSystems();
    m_engine.display.closeOpenWindows();
}

void InputExample::update(double time) {}

void InputExample::handleEvent(In<WindowShouldCloseEvent> event) {
    quit();
}

void InputExample::handleEvent(In<KeyPressEvent> event) {
    log().info("[Key Pressed] {}|{}", event.key_id, event.scancode);
}

void InputExample::handleEvent(In<KeyRepeatEvent> event) {
    log().info("[Key Repeated] {}|{}", event.key_id, event.scancode);
}

void InputExample::handleEvent(In<KeyReleaseEvent> event) {
    log().info("[Key Released] {}|{}", event.key_id, event.scancode);
}

void InputExample::handleEvent(In<TextInputEvent> event) {
    log().info("[TextInput] {}", (unsigned char)(event.character));
}

void InputExample::handleEvent(In<MouseButtonPressEvent> event) {
    log().info("[Mouse Button Pressed] {}", event.button_id);
}

void InputExample::handleEvent(In<MouseButtonRepeatEvent> event) {
    log().info("[Mouse Button Repeated] {}", event.button_id);
}

void InputExample::handleEvent(In<MouseButtonReleaseEvent> event) {
    log().info("[Mouse Button Released] {}", event.button_id);
}

void InputExample::handleEvent(In<CursorMovedEvent> event) {
    log().info("[Cursor Moved] {}", event.position);
}

void InputExample::handleEvent(In<CursorEnteredEvent> event) {
    log().info("[Cursor Entered] window[{}]", event.window->getWindowID());
}

void InputExample::handleEvent(In<CursorLeftEvent> event) {
    log().info("[Cursor Left] window[{}]", event.window->getWindowID());
}

void InputExample::handleEvent(In<ScrollEvent> event) {
    log().info("[Scroll] ({}; {})", event.x, event.y);
}

void InputExample::handleEvent(In<InputDeviceButtonPressEvent> event) {
    log().info("[ButtonPress] {}", event.button->getName());
}

void InputExample::handleEvent(In<InputDeviceButtonReleaseEvent> event) {
    log().info("[ButtonRelease] {}", event.button->getName());
}

void InputExample::pervCursor() {
    m_current_cursor = (m_current_cursor - 1) % m_cursor.size();

    CursorIcon* cursor = m_cursor[m_current_cursor].get();
    events::broadcast(kernel::SetCursorIconEvent{cursor, WindowId(0)});
}
void InputExample::nextCursor() {
    m_current_cursor = (m_current_cursor + 1) % m_cursor.size();

    CursorIcon* cursor = m_cursor[m_current_cursor].get();
    events::broadcast(kernel::SetCursorIconEvent{cursor, WindowId(0)});
}

} // namespace bembel
