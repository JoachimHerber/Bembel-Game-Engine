export module bembel.examples.input;

import bembel;

namespace bembel {
using namespace bembel::base;
using namespace bembel::kernel;

export class InputExample : public kernel::Application {
  public:
    InputExample();
    ~InputExample();

    virtual bool init() override;
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
    std::vector<Asset<CursorIcon>> m_cursor;
    uint                           m_current_cursor = 0;
};

} // namespace bembel
