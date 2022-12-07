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

    auto& asset_mgr = m_engine.assets;
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
    m_engine.display.closeOpenWindows();
}

void InputExample::update(double time) {}

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
    current_cursor = (current_cursor - 1) % cursor.size();

    CursorIcon* cursor = m_engine.assets.getAsset<CursorIcon>(this->cursor[current_cursor]);
    events::broadcast(kernel::SetCursorIconEvent{cursor, WindowId(0)});
}
void InputExample::nextCursor() {
    current_cursor = (current_cursor + 1) % cursor.size();

    CursorIcon* cursor = m_engine.assets.getAsset<CursorIcon>(this->cursor[current_cursor]);
    events::broadcast(kernel::SetCursorIconEvent{cursor, WindowId(0)});
}

} // namespace bembel
