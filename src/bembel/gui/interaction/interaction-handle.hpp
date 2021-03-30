#ifndef BEMBEL_GUI_INTERACTION_INTERACTIONHANDEL_HPP
#define BEMBEL_GUI_INTERACTION_INTERACTIONHANDEL_HPP

#include <bembel/base/utils/signal.hpp>
#include <bembel/kernel/assets/asset-handle.hpp>

namespace bembel::gui {
class Widget;

class BEMBEL_API InteractionHandle {
  public:
    InteractionHandle(Widget* widget);
    ~InteractionHandle();

    Widget* getWidget() const;

    const glm::ivec2& getPosition() const;
    void              setPosition(const glm::ivec2& pos);
    void              setPosition(int x, int y);

    const glm::ivec2& getSize() const;
    void              setSize(const glm::ivec2& size);
    void              setSize(int w, int h);

    enum class State {
        DISABLED,
        IDLE,
        SELECTED,
        ACTIVE,
    };

    State getState() const;

    bool isEnabled() const;
    bool isSelected() const;
    bool isActive() const;

    void disable();
    void enable();

    /**
     * This Signal is emitted when the state of the FocusHandel changes.
     * Signal Arguments:
     * 1. FocusHandle::State old_state
     * 2. FocusHandle::State new_stete
     */
    base::Signal<State, State> state_change_signal;

    /**
     * This Signal is emited when ever the curser moved while the the FocusHandle is active.
     * The Signal Argument is the accumulated curser movement (can be reset by signal handelers).
     */
    base::Signal<glm::ivec2&> dragging_signal;

    kernel::AssetHandle getCursor();
    void                setCursor(kernel::AssetHandle);

    // This methode is called by the focus dispatcher
    void setState(State);

  private:
    Widget* widget;

    glm::ivec2 position;
    glm::ivec2 size;

    State state;

    kernel::AssetHandle cursor;
};

} // namespace bembel::gui
#endif // include guards
