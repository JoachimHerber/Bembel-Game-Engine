#include "./input-handler.hpp"

#include <bembel/kernel/assets/asset-manager.hpp>
#include <bembel/kernel/display/cursor-icon.hpp>
#include <bembel/kernel/display/window.hpp>
#include <bembel/kernel/events/event-manager.hpp>

#include "../graphical-user-interface.hpp"
#include "../widgets/group-widget.hpp"

namespace bembel::gui {

InputHandler::InputHandler(GraphicalUserInterface* gui)
: gui{gui} {
}

InputHandler::~InputHandler() {
    if(this->button) {
        this->button->press_signal.removeSlot(this, &InputHandler::onButtonPress);
        this->button->release_signal.removeSlot(this, &InputHandler::onButtonRelease);
    }
}

void InputHandler::setButton(kernel::InputDeviceButton* button) {
    if(this->button) {
        this->button->press_signal.removeSlot(this, &InputHandler::onButtonPress);
        this->button->release_signal.removeSlot(this, &InputHandler::onButtonRelease);
    }
    this->button = button;
    if(this->button) {
        this->button->press_signal.addSlot(this, &InputHandler::onButtonPress);
        this->button->release_signal.addSlot(this, &InputHandler::onButtonRelease);
    }
}

void InputHandler::onCursorMoved(glm::ivec2 cursor_pos) {
    glm::ivec2 movement = cursor_pos - this->cursor_pos;
    this->cursor_pos    = cursor_pos;
    if(this->selected_handle && this->selected_handle->isActive()) {
        dragActiveHndl(movement);
    } else {
        updateSelectedHandle();
    }
}

void InputHandler::onButtonPress() {
    if(this->selected_handle != nullptr) {
        this->selected_handle->setState(InteractionHandle::State::ACTIVE);
        glm::vec2 hnld_pos = this->selected_handle->getPosition()
                             + this->selected_handle->getWidget()->position.get();

        this->dragging_offset.x = 0;
        this->dragging_offset.y = 0;
    }
}

void InputHandler::onButtonRelease() {
    if(this->selected_handle && this->selected_handle->isActive()) {
        InteractionHandle* focus_handle = findFocusedHndl();
        if(focus_handle == this->selected_handle) {
            this->selected_handle->setState(InteractionHandle::State::SELECTED);
        } else {
            this->selected_handle->setState(InteractionHandle::State::IDLE);

            setSelectedHandle(focus_handle);
        }
    }
}

void InputHandler::dragActiveHndl(glm::ivec2 cursor_movement) {
    this->dragging_offset += cursor_movement;

    this->selected_handle->dragging_signal.emit(this->dragging_offset);
}

void InputHandler::updateSelectedHandle() {
    InteractionHandle* focus_handle = findFocusedHndl();
    if(this->selected_handle != focus_handle) {
        if(this->selected_handle != nullptr)
            this->selected_handle->setState(InteractionHandle::State::IDLE);

        setSelectedHandle(focus_handle);
    }
}

InteractionHandle* InputHandler::findFocusedHndl() {
    InteractionHandle* focus_handle = nullptr;

    return this->findFocusedHndl(this->gui->getRootWidget(), glm::ivec2{0});
}

InteractionHandle* InputHandler::findFocusedHndl(Widget* widget, const glm::ivec2& offset) {
    InteractionHandle* focus_handle = nullptr;

    const glm::ivec2 min = offset + widget->position.get();
    const glm::ivec2 max = min + widget->size.get();

    if(this->cursor_pos.x < min.x || max.x < this->cursor_pos.x || this->cursor_pos.y < min.y
       || max.y < this->cursor_pos.y)
        return nullptr;

    for(auto& it : widget->getInteractionHandles()) {
        if(it->getState() == InteractionHandle::State::DISABLED) continue;

        const glm::ivec2 hndl_min = min + it->getPosition();
        const glm::ivec2 hndl_max = hndl_min + it->getSize();

        if(hndl_min.x <= this->cursor_pos.x && this->cursor_pos.x <= hndl_max.x
           && hndl_min.y <= this->cursor_pos.y && this->cursor_pos.y <= hndl_max.y) {
            focus_handle = it;
        }
    }

    for(auto& it : widget->getChildWidgets()) {
        auto* hndl = this->findFocusedHndl(it, min);
        if(hndl != nullptr) focus_handle = hndl;
    }
    return focus_handle;
}

void InputHandler::setSelectedHandle(InteractionHandle* handle) {
    this->selected_handle = handle;

    auto vp = this->gui->getView()->getViewport();
    if(!vp) return;
    auto window_id = vp->getWindow()->getWindowID();

    if(this->selected_handle != nullptr) {
        this->selected_handle->setState(InteractionHandle::State::SELECTED);

        kernel::CursorIcon* cursor = this->gui->getAssetManager().getAsset<kernel::CursorIcon>(
            this->selected_handle->getCursor());
        this->gui->getEventManager().broadcast(kernel::SetCursorIconEvent{cursor, window_id});
    } else {
        this->gui->getEventManager().broadcast(kernel::SetCursorIconEvent{nullptr, window_id});
    }
}

} // namespace bembel::gui
