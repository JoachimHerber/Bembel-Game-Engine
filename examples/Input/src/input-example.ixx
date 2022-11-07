export module bembel.examples.input;

import bembel;

namespace bembel {

export class InputExample : public kernel::Application {
  public:
    InputExample();
    ~InputExample();

    virtual bool init() override;
    virtual void cleanup() override;

    virtual void update(double time) override;

    void         handleEvent(const kernel::WindowShouldCloseEvent&);

    void         handleEvent(const kernel::KeyPressEvent&);
    void         handleEvent(const kernel::KeyRepeatEvent&);
    void         handleEvent(const kernel::KeyReleaseEvent&);
    void         handleEvent(const kernel::TextInputEvent&);
    void         handleEvent(const kernel::MouseButtonPressEvent&);
    void         handleEvent(const kernel::MouseButtonRepeatEvent&);
    void         handleEvent(const kernel::MouseButtonReleaseEvent&);
    void         handleEvent(const kernel::CursorMovedEvent&);
    void         handleEvent(const kernel::CursorEnteredEvent&);
    void         handleEvent(const kernel::CursorLeftEvent&);
    void         handleEvent(const kernel::ScrollEvent&);

    void         handleEvent(const kernel::InputDeviceButtonPressEvent&);
    void         handleEvent(const kernel::InputDeviceButtonReleaseEvent&);

    void         pervCursor();
    void         nextCursor();

  private:
    std::vector<kernel::AssetHandle> cursor;
    unsigned int                     current_cursor = 0;
};

} // namespace bembel
