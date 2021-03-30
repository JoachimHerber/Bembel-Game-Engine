#include "input-example.h"

#include <GLFW/glfw3.h>

#include <iostream>

namespace bembel {

InputExample::InputExample()
: kernel::Application() {
    kernel->getEventManager().addHandler<kernel::WindowShouldCloseEvent>(this);

    kernel->getEventManager().addHandler<kernel::KeyPressEvent>(this);
    kernel->getEventManager().addHandler<kernel::KeyRepeatEvent>(this);
    kernel->getEventManager().addHandler<kernel::KeyReleaseEvent>(this);
    kernel->getEventManager().addHandler<kernel::TextInputEvent>(this);
    kernel->getEventManager().addHandler<kernel::MouseButtonPressEvent>(this);
    kernel->getEventManager().addHandler<kernel::MouseButtonRepeatEvent>(this);
    kernel->getEventManager().addHandler<kernel::MouseButtonReleaseEvent>(this);
    kernel->getEventManager().addHandler<kernel::CursorMovedEvent>(this);
    kernel->getEventManager().addHandler<kernel::CursorEnteredEvent>(this);
    kernel->getEventManager().addHandler<kernel::CursorLeftEvent>(this);
    kernel->getEventManager().addHandler<kernel::ScrollEvent>(this);

    kernel->getEventManager().addHandler<kernel::InputDeviceButtonPressEvent>(this);
    kernel->getEventManager().addHandler<kernel::InputDeviceButtonReleaseEvent>(this);
}

InputExample::~InputExample() {
}

bool InputExample::init() {
    auto window = kernel->getDisplayManager().createWindow();
    window->open("Input Example");
    kernel->initSystems();

    auto& keyboard = kernel->getInputManager().getKeyboard();

    keyboard.getKey(GLFW_KEY_LEFT)->release_signal.addSlot(this, &InputExample::pervCursor);
    keyboard.getKey(GLFW_KEY_RIGHT)->release_signal.addSlot(this, &InputExample::nextCursor);

    auto& asset_mgr = kernel->getAssetManager();
    cursor.push_back(asset_mgr.getAssetHandle<kernel::CursorIcon>("Arrow"));
    cursor.push_back(asset_mgr.getAssetHandle<kernel::CursorIcon>("IBeam"));
    cursor.push_back(asset_mgr.getAssetHandle<kernel::CursorIcon>("Crosshair"));
    cursor.push_back(asset_mgr.getAssetHandle<kernel::CursorIcon>("Hand"));
    cursor.push_back(asset_mgr.getAssetHandle<kernel::CursorIcon>("HResize"));
    cursor.push_back(asset_mgr.getAssetHandle<kernel::CursorIcon>("VResize"));
    return true;
}

void InputExample::cleanup() {
    kernel->shutdownSystems();
    kernel->getDisplayManager().closeOpenWindows();
}

void InputExample::update(double time) {
}

void InputExample::handleEvent(const kernel::WindowShouldCloseEvent& event) {
    quit();
}

void InputExample::handleEvent(const kernel::KeyPressEvent& event) {
    std::cout << "[Key Pressed] " << event.key_id << "|" << event.scancode << std::endl;
}

void InputExample::handleEvent(const kernel::KeyRepeatEvent& event) {
    std::cout << "[Key Repeated] " << event.key_id << "|" << event.scancode << std::endl;
}

void InputExample::handleEvent(const kernel::KeyReleaseEvent& event) {
    std::cout << "[Key Released] " << event.key_id << "|" << event.scancode << std::endl;
}

void InputExample::handleEvent(const kernel::TextInputEvent& event) {
    std::cout << "[TextInput] '" << (unsigned char)(event.character) << "'" << std::endl;
}

void InputExample::handleEvent(const kernel::MouseButtonPressEvent& event) {
    std::cout << "[Mouse Button Pressed] " << event.button_id << std::endl;
}

void InputExample::handleEvent(const kernel::MouseButtonRepeatEvent& event) {
    std::cout << "[Mouse Button Repeated] " << event.button_id << std::endl;
}

void InputExample::handleEvent(const kernel::MouseButtonReleaseEvent& event) {
    std::cout << "[Mouse Button Released] " << event.button_id << std::endl;
}

void InputExample::handleEvent(const kernel::CursorMovedEvent& event) {
    std::cout << "[Cursor Moved] (" << event.position.x << ";" << event.position.y << ")"
              << std::endl;
}

void InputExample::handleEvent(const kernel::CursorEnteredEvent& event) {
    std::cout << "[Cursor Entered] window[" << event.window->getWindowID() << "]" << std::endl;
}

void InputExample::handleEvent(const kernel::CursorLeftEvent& event) {
    std::cout << "[Cursor Left] window[" << event.window->getWindowID() << "]" << std::endl;
}

void InputExample::handleEvent(const kernel::ScrollEvent& event) {
    std::cout << "[Scroll] (" << event.x << ";" << event.y << ")" << std::endl;
}

void InputExample::handleEvent(const kernel::InputDeviceButtonPressEvent& event) {
    std::cout << "[ButtonPress] " << event.button->getName() << std::endl;
}

void InputExample::handleEvent(const kernel::InputDeviceButtonReleaseEvent& event) {
    std::cout << "[ButtonRelease] " << event.button->getName() << std::endl;
}

void InputExample::pervCursor() {
    current_cursor = (current_cursor - 1) % cursor.size();

    auto& asset_mgr = kernel->getAssetManager();
    auto  cursor    = asset_mgr.getAsset<kernel::CursorIcon>(this->cursor[current_cursor]);
    auto& event_mgr = kernel->getEventManager();
    event_mgr.broadcast(kernel::SetCursorIconEvent{cursor, 0});
}
void InputExample::nextCursor() {
    current_cursor = (current_cursor + 1) % cursor.size();

    auto& asset_mgr = kernel->getAssetManager();
    auto  cursor    = asset_mgr.getAsset<kernel::CursorIcon>(this->cursor[current_cursor]);
    auto& event_mgr = kernel->getEventManager();
    event_mgr.broadcast(kernel::SetCursorIconEvent{cursor, 0});
}

} // namespace bembel
