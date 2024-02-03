module;
#include <array>
#include <string_view>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
module bembel.kernel.input;

import bembel.base;

namespace bembel::kernel {
using namespace bembel::base;

// clang-format off
static const std::map<Keyboard::KeyId, std::string_view> KEY_NAMES = {
    {Keyboard::SPACE,         "[Space]"},        {Keyboard::APOSTROPHE,   "'"},
    {Keyboard::COMMA,         ","},              {Keyboard::MINUS,        "-"},
    {Keyboard::PERIOD,        "."},              {Keyboard::SLASH,        "/"},
    {Keyboard::SEMICOLON,     ";"},              {Keyboard::EQUAL,        "="},
    {Keyboard::_0,            "0"},              {Keyboard::_1,           "1"},
    {Keyboard::_2,            "2"},              {Keyboard::_3,           "3"},
    {Keyboard::_4,            "4"},              {Keyboard::_5,           "5"},
    {Keyboard::_6,            "6"},              {Keyboard::_7,           "7"},
    {Keyboard::_8,            "8"},              {Keyboard::_9,           "9"},
    {Keyboard::A,             "A"},              {Keyboard::B,            "B"},
    {Keyboard::C,             "C"},              {Keyboard::D,            "D"},
    {Keyboard::E,             "E"},              {Keyboard::F,            "F"},
    {Keyboard::G,             "G"},              {Keyboard::H,            "H"},
    {Keyboard::I,             "I"},              {Keyboard::J,            "J"},
    {Keyboard::K,             "K"},              {Keyboard::L,            "L"},
    {Keyboard::M,             "M"},              {Keyboard::N,            "N"},
    {Keyboard::O,             "O"},              {Keyboard::P,            "P"},
    {Keyboard::Q,             "Q"},              {Keyboard::R,            "R"},
    {Keyboard::S,             "S"},              {Keyboard::T,            "T"},
    {Keyboard::U,             "U"},              {Keyboard::V,            "V"},
    {Keyboard::W,             "W"},              {Keyboard::X,            "X"},
    {Keyboard::Y,             "Y"},              {Keyboard::Z,            "Z"},
    {Keyboard::LEFT_BRACKET,  "["},              {Keyboard::BACKSLASH,    "\\"},
    {Keyboard::RIGHT_BRACKET, "]"},              {Keyboard::GRAVE_ACCENT, "´"},
    {Keyboard::WORLD_1,       "[non-US #1]"},    {Keyboard::WORLD_2,      "[non-US #2]"},
    {Keyboard::ESCAPE,        "[Esc]"},          {Keyboard::ENTER,        "[Enter]"},
    {Keyboard::TAB,           "[Tab]"},          {Keyboard::BACKSPACE,    "[BackSpace]"},
    {Keyboard::INSERT,        "[Insert]"},       {Keyboard::DELETE,       "[Delete]"},
    {Keyboard::RIGHT,         "[Right]"},        {Keyboard::LEFT,         "[Left]"},
    {Keyboard::DOWN,          "[Down]"},         {Keyboard::UP,           "[Up]"},
    {Keyboard::PAGE_UP,       "[PageUp]"},       {Keyboard::PAGE_DOWN,    "[PageDown]"},
    {Keyboard::HOME,          "[Home]"},         {Keyboard::END,          "[End]"},
    {Keyboard::CAPS_LOCK,     "[CapsLock]"},     {Keyboard::SCROLL_LOCK,  "[ScrollLock]"},
    {Keyboard::NUM_LOCK,      "[NumLock]"},      {Keyboard::PRINT_SCREEN, "[PrintScreen]"},
    {Keyboard::PAUSE,         "[Pause]"},        {Keyboard::F1,           "[F1]"},
    {Keyboard::F2,            "[F2]"},           {Keyboard::F3,           "[F3]"},
    {Keyboard::F4,            "[F4]"},           {Keyboard::F5,           "[F5]"},
    {Keyboard::F6,            "[F6]"},           {Keyboard::F7,           "[F7]"},
    {Keyboard::F8,            "[F8]"},           {Keyboard::F9,           "[F9]"},
    {Keyboard::F10,           "[F10]"},          {Keyboard::F11,          "[F11]"},
    {Keyboard::F12,           "[F12]"},          {Keyboard::F13,          "[F13]"},
    {Keyboard::F14,           "[F14]"},          {Keyboard::F15,          "[F15]"},
    {Keyboard::F16,           "[F16]"},          {Keyboard::F17,          "[F17]"},
    {Keyboard::F18,           "[F18]"},          {Keyboard::F19,          "[F19]"},
    {Keyboard::F20,           "[F20]"},          {Keyboard::F21,          "[F21]"},
    {Keyboard::F22,           "[F22]"},          {Keyboard::F23,          "[F23]"},
    {Keyboard::F24,           "[F24]"},          {Keyboard::F25,          "[F25]"},
    {Keyboard::KP_0,          "[NumPad 0]"},     {Keyboard::KP_1,         "[NumPad 1]"},
    {Keyboard::KP_2,          "[NumPad 2]"},     {Keyboard::KP_3,         "[NumPad 3]"},
    {Keyboard::KP_4,          "[NumPad 4]"},     {Keyboard::KP_5,         "[NumPad 5]"},
    {Keyboard::KP_6,          "[NumPad 6]"},     {Keyboard::KP_7,         "[NumPad 7]"},
    {Keyboard::KP_8 ,         "[NumPad 8]"},     {Keyboard::KP_9,         "[NumPad 9]"},
    {Keyboard::KP_DECIMAL,    "[NumPad .]"},     {Keyboard::KP_DIVIDE,    "[NumPad /]"},
    {Keyboard::KP_MULTIPLY,   "[NumPad *]"},     {Keyboard::KP_SUBTRACT,  "[NumPad -]"},
    {Keyboard::KP_ADD,        "[NumPad +]"},     {Keyboard::KP_ENTER,     "[NumPad Enter]"},
    {Keyboard::KP_EQUAL,      "[NumPad =]"},     {Keyboard::LEFT_SHIFT,   "[L Shift]"},
    {Keyboard::LEFT_CONTROL,  "[L Ctl]"},        {Keyboard::LEFT_ALT,     "[L Alt]"},
    {Keyboard::LEFT_SUPER,    "[L Super]"},      {Keyboard::RIGHT_SHIFT,  "[R Shift]"},
    {Keyboard::RIGHT_CONTROL, "[R Ctl]"},        {Keyboard::RIGHT_ALT,    "[R Alt]"},
    {Keyboard::RIGHT_SUPER,   "[R Super]"},      {Keyboard::MENU,         "[MENU]"}
};
// clang-format on

Keyboard::Keyboard() : InputDevice("Keyboard") {}

Keyboard::~Keyboard() {}

void Keyboard::initDefaultKeys() {
    // register keys
    // clang-format off
    constexpr auto GLFW_KEYS = std::array{
        SPACE, APOSTROPHE, COMMA, MINUS, PERIOD, SLASH, SEMICOLON, EQUAL,
        _0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        LEFT_BRACKET, BACKSLASH, RIGHT_BRACKET, GRAVE_ACCENT, 
        ESCAPE, ENTER, TAB, BACKSPACE, INSERT, DELETE,
        RIGHT, LEFT, DOWN, UP, PAGE_UP, PAGE_DOWN, HOME, END,
        CAPS_LOCK, SCROLL_LOCK, NUM_LOCK, PRINT_SCREEN, PAUSE,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,
        KP_0, KP_1, KP_2, KP_3, KP_4, KP_5, KP_6, KP_7, KP_8, KP_9,
        KP_DECIMAL, KP_DIVIDE, KP_MULTIPLY, KP_SUBTRACT, KP_ADD, KP_ENTER, KP_EQUAL,
        LEFT_SHIFT,  LEFT_CONTROL,  LEFT_ALT,  LEFT_SUPER,
        RIGHT_SHIFT, RIGHT_CONTROL, RIGHT_ALT, RIGHT_SUPER,
        /*WORLD_1, WORLD_2,*/ MENU};
    // clang-format on
    for(KeyId key_id : GLFW_KEYS) { createButton(key_id, 0); }
}

Keyboard::Key* Keyboard::getKey(In<KeyId> key_id, In<Scancode> scancode) {
    if(key_id != GLFW_KEY_UNKNOWN) {
        auto it = m_known_keys.find(key_id);
        if(it != m_known_keys.end()) return it->second;
    } else {
        auto it = m_unknown_keys.find(scancode);
        if(it != m_unknown_keys.end()) return it->second;
    }

    return createButton(key_id, scancode);
}

InputDevice::Button* Keyboard::getButton(In<std::string_view> name) {
    for(auto& key : m_keys) {
        if(key.getName() == name) return &key;
    }
    return nullptr;
}

void Keyboard::handleEvent(In<KeyPressEvent> event) {
    auto key = this->getKey(KeyId(event.key_id), event.scancode);
    events::broadcast<InputDeviceButtonPressEvent>(key);
    key->setIsPressed(true);
}

void Keyboard::handleEvent(In<KeyReleaseEvent> event) {
    auto key = this->getKey(KeyId(event.key_id), event.scancode);
    events::broadcast<InputDeviceButtonReleaseEvent>(key);
    key->setIsPressed(false);
}

Keyboard::Key* Keyboard::createButton(In<KeyId> key_id, In<Scancode> scancode) {
    if(m_keys.size() == MAX_NUM_KEYS) {
        logError("Max number of Keys for Keyboard reached");
        return nullptr;
    }

    char const* keyName = glfwGetKeyName(int(key_id), scancode);

    m_keys = std::span((Key*)m_Buttons, m_keys.size() + 1);

    Key* key = &m_keys.back();

    if(keyName != nullptr) {
        new(key) Button(this, keyName);
    } else {
        auto it = KEY_NAMES.find(key_id);
        if(it != KEY_NAMES.end()) {
            new(key) Button(this, it->second);
        } else {
            char name[256];
            sprintf(name, "[KEY%d]", scancode);
            new(key) Button(this, name);
        }
    }

    if(key_id != GLFW_KEY_UNKNOWN) {
        this->m_known_keys.emplace(key_id, key);
    } else {
        this->m_unknown_keys.emplace(scancode, key);
    }
    return key;
}

} // namespace bembel::kernel
