#include "./keyboard.hpp"

#include <GLFW/glfw3.h>

namespace bembel::kernel {

Keyboard::Keyboard(EventManager& eventMgr)
: InputDevice(eventMgr, "Keyboard") {
    this->event_mgr.addHandler<KeyPressEvent>(this);
    this->event_mgr.addHandler<KeyReleaseEvent>(this);

    // register names for non printable keys
    this->initKey(GLFW_KEY_SPACE, "[SPACE]");
    this->initKey(GLFW_KEY_ESCAPE, "[ESC]");
    this->initKey(GLFW_KEY_ENTER, "[ENTER]");
    this->initKey(GLFW_KEY_TAB, "[TAB]");
    this->initKey(GLFW_KEY_BACKSPACE, "[BACKSPACE]");
    this->initKey(GLFW_KEY_INSERT, "[INSERT]");
    this->initKey(GLFW_KEY_DELETE, "[DELETE]");
    this->initKey(GLFW_KEY_RIGHT, "[RIGHT]");
    this->initKey(GLFW_KEY_LEFT, "[LEFT]");
    this->initKey(GLFW_KEY_DOWN, "[DOWN]");
    this->initKey(GLFW_KEY_UP, "[UP]");
    this->initKey(GLFW_KEY_PAGE_UP, "[PAGE_UP]");
    this->initKey(GLFW_KEY_PAGE_DOWN, "[PAGE_DOWN]");
    this->initKey(GLFW_KEY_HOME, "[PAGE_HOME]");
    this->initKey(GLFW_KEY_END, "[END]");
    this->initKey(GLFW_KEY_CAPS_LOCK, "[CAPS_LOCK]");
    this->initKey(GLFW_KEY_SCROLL_LOCK, "[SCROLL_LOCK]");
    this->initKey(GLFW_KEY_NUM_LOCK, "[NUM_LOCK]");
    this->initKey(GLFW_KEY_PRINT_SCREEN, "[PRINT]");
    this->initKey(GLFW_KEY_PAUSE, "[PAUSE]");
    this->initKey(GLFW_KEY_F1, "[F1]");
    this->initKey(GLFW_KEY_F2, "[F2]");
    this->initKey(GLFW_KEY_F3, "[F3]");
    this->initKey(GLFW_KEY_F4, "[F4]");
    this->initKey(GLFW_KEY_F5, "[F5]");
    this->initKey(GLFW_KEY_F6, "[F6]");
    this->initKey(GLFW_KEY_F7, "[F7]");
    this->initKey(GLFW_KEY_F8, "[F8]");
    this->initKey(GLFW_KEY_F9, "[F9]");
    this->initKey(GLFW_KEY_F10, "[F10]");
    this->initKey(GLFW_KEY_F11, "[F11]");
    this->initKey(GLFW_KEY_F12, "[F12]");
    this->initKey(GLFW_KEY_F13, "[F13]");
    this->initKey(GLFW_KEY_F14, "[F14]");
    this->initKey(GLFW_KEY_F15, "[F15]");
    this->initKey(GLFW_KEY_F16, "[F16]");
    this->initKey(GLFW_KEY_F17, "[F17]");
    this->initKey(GLFW_KEY_F18, "[F18]");
    this->initKey(GLFW_KEY_F19, "[F19]");
    this->initKey(GLFW_KEY_F20, "[F20]");
    this->initKey(GLFW_KEY_F21, "[F21]");
    this->initKey(GLFW_KEY_F22, "[F22]");
    this->initKey(GLFW_KEY_F23, "[F23]");
    this->initKey(GLFW_KEY_F24, "[F24]");
    this->initKey(GLFW_KEY_F25, "[F25]");
    this->initKey(GLFW_KEY_KP_0, "[NUM_PAD_0]");
    this->initKey(GLFW_KEY_KP_1, "[NUM_PAD_1]");
    this->initKey(GLFW_KEY_KP_2, "[NUM_PAD_2]");
    this->initKey(GLFW_KEY_KP_3, "[NUM_PAD_3]");
    this->initKey(GLFW_KEY_KP_4, "[NUM_PAD_4]");
    this->initKey(GLFW_KEY_KP_5, "[NUM_PAD_5]");
    this->initKey(GLFW_KEY_KP_6, "[NUM_PAD_6]");
    this->initKey(GLFW_KEY_KP_7, "[NUM_PAD_7]");
    this->initKey(GLFW_KEY_KP_8, "[NUM_PAD_8]");
    this->initKey(GLFW_KEY_KP_9, "[NUM_PAD_9]");
    this->initKey(GLFW_KEY_KP_DECIMAL, "[NUM_PAD_DECIMAL]");
    this->initKey(GLFW_KEY_KP_DIVIDE, "[NUM_PAD_DIVIDE]");
    this->initKey(GLFW_KEY_KP_MULTIPLY, "[NUM_PAD_MULTIPLY]");
    this->initKey(GLFW_KEY_KP_SUBTRACT, "[NUM_PAD_SUBTRACT]");
    this->initKey(GLFW_KEY_KP_ADD, "[NUM_PAD_ADD]");
    this->initKey(GLFW_KEY_KP_ENTER, "[NUM_PAD_ENTER]");
    this->initKey(GLFW_KEY_KP_EQUAL, "[NUM_PAD_EQUAL]");
    this->initKey(GLFW_KEY_LEFT_SHIFT, "[SHIFT]");
    this->initKey(GLFW_KEY_LEFT_CONTROL, "[CONTROL]");
    this->initKey(GLFW_KEY_LEFT_ALT, "[ALT]");
    this->initKey(GLFW_KEY_RIGHT_SHIFT, "[RIGHT_SHIFT]");
    this->initKey(GLFW_KEY_RIGHT_CONTROL, "[RIGHT_CONTROL]");
    this->initKey(GLFW_KEY_RIGHT_ALT, "[RIGHT_ALT]");
    this->initKey(GLFW_KEY_MENU, "[MENU]");
}

Keyboard::~Keyboard() {
    this->event_mgr.removeHandler<KeyPressEvent>(this);
    this->event_mgr.removeHandler<KeyReleaseEvent>(this);
}

InputDeviceButton* Keyboard::getKey(int key_id, int scancode) {
    if(key_id != GLFW_KEY_UNKNOWN) scancode = 0;

    auto key = std::make_pair(key_id, scancode);
    auto it  = this->key_mapping.find(key);

    if(it != this->key_mapping.end()) return this->buttons[it->second].get();

    auto index = this->buttons.size();

    const char* keyName = glfwGetKeyName(key_id, scancode);
    if(keyName != nullptr) {
        this->buttons.push_back(std::make_unique<InputDeviceButton>(this, keyName));
    } else {
        char name[256];
        sprintf(name, "[KEY%d]", scancode);
        this->buttons.push_back(std::make_unique<InputDeviceButton>(this, name));
    }
    this->key_mapping.emplace(key, int(index));

    return this->buttons.back().get();
}

void Keyboard::handleEvent(const kernel::KeyPressEvent& event) {
    auto key = this->getKey(event.key_id, event.scancode);
    this->event_mgr.broadcast(InputDeviceButtonPressEvent{key});
    key->setIsPressed(true);
}

void Keyboard::handleEvent(const kernel::KeyReleaseEvent& event) {
    auto key = this->getKey(event.key_id, event.scancode);
    this->event_mgr.broadcast(InputDeviceButtonReleaseEvent{key});
    key->setIsPressed(false);
}

void Keyboard::initKey(int key_id, const std::string& name) {
    auto key = std::make_pair(key_id, 0);
    auto it  = this->key_mapping.find(key);

    unsigned index = unsigned(this->buttons.size());

    this->buttons.push_back(std::make_unique<InputDeviceButton>(this, name));
    this->key_mapping.emplace(key, index);
}

} // namespace bembel::kernel
