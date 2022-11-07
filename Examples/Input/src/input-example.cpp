module;
#include <string>
module bembel.examples.input;

namespace bembel {
using namespace bembel::base;
using namespace bembel::kernel;

InputExample::InputExample() : Application() {
    m_engine.getEventManager().addHandler<WindowShouldCloseEvent>(this);

    m_engine.getEventManager().addHandler<KeyPressEvent>(this);
    m_engine.getEventManager().addHandler<KeyRepeatEvent>(this);
    m_engine.getEventManager().addHandler<KeyReleaseEvent>(this);
    m_engine.getEventManager().addHandler<TextInputEvent>(this);
    m_engine.getEventManager().addHandler<MouseButtonPressEvent>(this);
    m_engine.getEventManager().addHandler<MouseButtonRepeatEvent>(this);
    m_engine.getEventManager().addHandler<MouseButtonReleaseEvent>(this);
    m_engine.getEventManager().addHandler<CursorMovedEvent>(this);
    m_engine.getEventManager().addHandler<CursorEnteredEvent>(this);
    m_engine.getEventManager().addHandler<CursorLeftEvent>(this);
    m_engine.getEventManager().addHandler<ScrollEvent>(this);

    m_engine.getEventManager().addHandler<InputDeviceButtonPressEvent>(this);
    m_engine.getEventManager().addHandler<InputDeviceButtonReleaseEvent>(this);
}

InputExample::~InputExample() {
}

bool InputExample::init() {
    auto window = m_engine.getDisplayManager().createWindow();
    window->open("Input Example");
    m_engine.initSystems();

    auto& keyboard = m_engine.getInputManager().getKeyboard();

    keyboard.getKey(Keyboard::LEFT)->release_signal.bind(this, &InputExample::pervCursor);
    keyboard.getKey(Keyboard::RIGHT)->release_signal.bind(this, &InputExample::nextCursor);

    auto& asset_mgr = m_engine.getAssetManager();
    cursor.push_back(asset_mgr.getAssetHandle<CursorIcon>("Arrow"));
    cursor.push_back(asset_mgr.getAssetHandle<CursorIcon>("IBeam"));
    cursor.push_back(asset_mgr.getAssetHandle<CursorIcon>("Crosshair"));
    cursor.push_back(asset_mgr.getAssetHandle<CursorIcon>("Hand"));
    cursor.push_back(asset_mgr.getAssetHandle<CursorIcon>("HResize"));
    cursor.push_back(asset_mgr.getAssetHandle<CursorIcon>("VResize"));
    return true;
}

void InputExample::cleanup() {
    m_engine.shutdownSystems();
    m_engine.getDisplayManager().closeOpenWindows();
}

void InputExample::update(double time) {
}

void InputExample::handleEvent(const WindowShouldCloseEvent& event) {
    quit();
}

void InputExample::handleEvent(const KeyPressEvent& event) {
    log().info("[Key Pressed] {}|{}", event.key_id, event.scancode);
}

void InputExample::handleEvent(const KeyRepeatEvent& event) {
    log().info("[Key Repeated] {}|{}", event.key_id, event.scancode);
}

void InputExample::handleEvent(const KeyReleaseEvent& event) {
    log().info("[Key Released] {}|{}", event.key_id, event.scancode);
}

void InputExample::handleEvent(const TextInputEvent& event) {
    log().info("[TextInput] {}", (unsigned char)(event.character));
}

void InputExample::handleEvent(const MouseButtonPressEvent& event) {
    log().info("[Mouse Button Pressed] {}", event.button_id);
}

void InputExample::handleEvent(const MouseButtonRepeatEvent& event) {
    log().info("[Mouse Button Repeated] {}", event.button_id);
}

void InputExample::handleEvent(const MouseButtonReleaseEvent& event) {
    log().info("[Mouse Button Released] {}", event.button_id);
}

void InputExample::handleEvent(const CursorMovedEvent& event) {
    log().info("[Cursor Moved] {}", event.position);
}

void InputExample::handleEvent(const CursorEnteredEvent& event) {
    log().info("[Cursor Entered] window[{}]", event.window->getWindowID());
}

void InputExample::handleEvent(const CursorLeftEvent& event) {
    log().info("[Cursor Left] window[{}]", event.window->getWindowID());
}

void InputExample::handleEvent(const ScrollEvent& event) {
    log().info("[Scroll] ({}; {})", event.x, event.y);
}

void InputExample::handleEvent(const InputDeviceButtonPressEvent& event) {
    log().info("[ButtonPress] {}", event.button->getName());
}

void InputExample::handleEvent(const InputDeviceButtonReleaseEvent& event) {
    log().info("[ButtonRelease] {}", event.button->getName());
}

void InputExample::pervCursor() {
    current_cursor        = (current_cursor - 1) % cursor.size();

    auto&       asset_mgr = m_engine.getAssetManager();
    CursorIcon* cursor    = asset_mgr.getAsset<CursorIcon>(this->cursor[current_cursor]);
    auto&       event_mgr = m_engine.getEventManager();
    event_mgr.broadcast(kernel::SetCursorIconEvent{cursor, WindowId(0) });
}
void InputExample::nextCursor() {
    current_cursor        = (current_cursor + 1) % cursor.size();

    auto&       asset_mgr = m_engine.getAssetManager();
    CursorIcon* cursor    = asset_mgr.getAsset<CursorIcon>(this->cursor[current_cursor]);
    auto&       event_mgr = m_engine.getEventManager();
    event_mgr.broadcast(kernel::SetCursorIconEvent{cursor, WindowId(0)});
}

} // namespace bembel
