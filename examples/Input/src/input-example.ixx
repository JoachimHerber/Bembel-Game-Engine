﻿module;
#include <string_view>
#include <span>
export module bembel.examples.input;

import bembel;

namespace bembel {
using namespace bembel::base;
using namespace bembel::kernel;

export class InputExample
  : public kernel::Application<>
{
  public:
    InputExample();
    ~InputExample();

    virtual bool init(std::span<std::string_view> args) override;
    virtual void cleanup() override;

    virtual void update(double time) override;

    void handleEvent(In<WindowShouldCloseEvent>);

    void handleEvent(In<KeyPressEvent>);
    void handleEvent(In<KeyRepeatEvent>);
    void handleEvent(In<KeyReleaseEvent>);
    void handleEvent(In<TextInputEvent>);
    void handleEvent(In<MouseButtonPressEvent>);
    void handleEvent(In<MouseButtonRepeatEvent>);
    void handleEvent(In<MouseButtonReleaseEvent>);
    void handleEvent(In<CursorMovedEvent>);
    void handleEvent(In<CursorEnteredEvent>);
    void handleEvent(In<CursorLeftEvent>);
    void handleEvent(In<ScrollEvent>);

    void handleEvent(In<InputDeviceButtonPressEvent>);
    void handleEvent(In<InputDeviceButtonReleaseEvent>);

    void pervCursor();
    void nextCursor();

  private:
    EventHandlerGuard<
        WindowShouldCloseEvent,
        KeyPressEvent,
        KeyRepeatEvent,
        KeyReleaseEvent,
        TextInputEvent,
        MouseButtonPressEvent,
        MouseButtonRepeatEvent,
        MouseButtonReleaseEvent,
        CursorMovedEvent,
        CursorEnteredEvent,
        CursorLeftEvent,
        ScrollEvent,
        InputDeviceButtonPressEvent,
        InputDeviceButtonReleaseEvent>
        m_guard{this};

    std::vector<Asset<CursorIcon>> m_cursor;
    uint                           m_current_cursor = 0;
};

} // namespace bembel
