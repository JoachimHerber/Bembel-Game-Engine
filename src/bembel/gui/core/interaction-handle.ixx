export module bembel.gui.core : InteractionHandle;

import bembel.base;
import bembel.kernel;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class InteractionHandle {
  public:
    InteractionHandle()  = default;
    ~InteractionHandle() = default;

    bool isDisabled() const { return m_disabled; }
    bool isHovered() const { return m_hovered; }
    bool isSelected() const { return m_selected; }

    void disable() { m_disabled = true; }
    void enable() { m_disabled = false; }

    // This methode is called by bembel::gui::InputHandler
    void setIsSelected(bool selected) {
        if(m_selected == selected) return;

        selection_signal.emit(selected);
        m_selected = selected;
    }

    void setIsHovered(bool hovered) {
        if(m_hovered == hovered) return;

        hover_signal.emit(hovered);
        m_hovered = hovered;
    }

    enum class Action {
        INTERACT,
        DELETE,
        BACKSPACE,
        NAVIGATE_RIGHT,
        NAVIGATE_LEFT,
        NAVIGATE_UP,
        NAVIGATE_DOWN
    };

  public:
    /**
     * This Signal is emited when ever the curser is moved while the the InteractionHandle is
     * active. 
     * Fist Signal Argument: Cursor Position (relative to Widget)
     * Second Signal Argument: Accumulated curser movement (can be modified by signal handelers).
     */
    Signal<ivec2, ivec2&> dragging_signal;

    /**
     * This Signal is emited when the hovered state (whether the curser is above the
     * InteractionHandle or not) of the InteractionHandle chages.
     */
    Signal<bool>          hover_signal;
    Signal<bool>          selection_signal;

    Signal<ivec2>         press_signal;
    Signal<ivec2>         release_signal;

    Signal<Action, ivec2> action_signal;

    Signal<char32_t>      text_input_signal;

    ivec2                 position{0, 0};
    ivec2                 size{1, 1};

    AssetHandle           cursor;

  private:
    bool m_disabled = false;
    bool m_hovered  = false;
    bool m_selected = false;
};

} // namespace bembel::gui