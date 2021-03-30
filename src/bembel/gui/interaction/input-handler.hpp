#ifndef BEMBEL_GUI_INTERACTION_INPUTHANDLER_HPP
#define BEMBEL_GUI_INTERACTION_INPUTHANDLER_HPP

#include <bembel/kernel/events/input-events.hpp>
#include <bembel/kernel/input/input-device-button.hpp>

namespace bembel::gui {
class GraphicalUserInterface;
class InteractionHandle;
class Widget;

class BEMBEL_API InputHandler {
  public:
    InputHandler(GraphicalUserInterface* gui);
    InputHandler(const InputHandler&) = delete;
    InputHandler& operator=(const InputHandler&) = delete;
    ~InputHandler();

    void setButton(kernel::InputDeviceButton*);

    void onCursorMoved(glm::ivec2 pos);

  protected:
    void onButtonPress();
    void onButtonRelease();

    void dragActiveHndl(glm::ivec2 cursor_movement);
    void updateSelectedHandle();

    InteractionHandle* findFocusedHndl();
    InteractionHandle* findFocusedHndl(Widget*, const glm::ivec2&);

    void setSelectedHandle(InteractionHandle*);

  private:
    GraphicalUserInterface*    gui;
    kernel::InputDeviceButton* button = nullptr;

    glm::ivec2 cursor_pos;
    glm::ivec2 dragging_offset;

    InteractionHandle* selected_handle = nullptr;
};

} // namespace bembel::gui
#endif // include guards
