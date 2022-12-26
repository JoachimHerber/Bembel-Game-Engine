module;
#include <array>
export module bembel.gui.core:InputHandler;

import bembel.base;
import bembel.kernel;

import :InteractionHandle;
import :Widget;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class InputHandler {
  public:
    InputHandler( Widget& root_widget, Viewport::View& gui_view);
    InputHandler(const InputHandler&)            = delete;
    InputHandler& operator=(const InputHandler&) = delete;
    ~InputHandler();

    using Action = InteractionHandle::Action;
    void setButton(Action, InputDevice::Button*);
    void setButtons(
        InputDevice::Button* interact,
        InputDevice::Button* del,
        InputDevice::Button* backspace,
        InputDevice::Button* navigate_right,
        InputDevice::Button* navigate_left,
        InputDevice::Button* navigate_up,
        InputDevice::Button* navigate_down
    );

    void onCursorMoved(ivec2);

    void handleEvent(In<TextInputEvent>);

  protected:
    void onButtonPress();
    void onButtonRelease();

    void onDelete();
    void onBackspace();
    void onNavigateRight();
    void onNavigateLeft();
    void onNavigateUp();
    void onNavigateDown();

    void updateFocus();

    struct Handle {
        Widget*            widget = nullptr;
        InteractionHandle* handle = nullptr;

             operator bool() { return handle != nullptr && widget != nullptr; }
        bool operator==(const Handle& other) {
            return handle == other.handle && widget == other.widget;
        }
        bool operator!=(const Handle& other) {
            return handle != other.handle || widget != other.widget;
        }
    };

    Handle findFocusedHndl();
    Handle findFocusedHndl(Widget&, const ivec2&);

    void setFocus(Handle&);
    void setSelection(Handle&);

    void updateCursorIcon();

    ivec2 getRelativeCursorPos(Handle&);

  private:
    using Buttons = std::array<InputDevice::Button*, 7>;

    Widget&         m_root_widget;
    Viewport::View& m_gui_view; // the view of the gui, used to access the viewport/window

    Buttons m_buttons = {nullptr};

    ivec2 m_cursor_pos      = {0, 0};
    bool  m_dragging        = false;
    ivec2 m_dragging_offset = {0, 0};

    Handle m_focus;
    Handle m_selection;
};

} // namespace bembel::gui