module;
#include "bembel/pch.h"
module bembel.gui.core;

import bembel.base;
import bembel.kernel;

import :InteractionHandle;
import :Widget;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

InputHandler::InputHandler(
    EventManager& event_mgr, AssetManager& asset_mgr, Widget& root_widget, Viewport::View& gui_view
)
  : m_event_mgr{event_mgr}
  , m_asset_mgr{asset_mgr}
  , m_root_widget{root_widget}
  , m_gui_view{gui_view} {
    m_event_mgr.addHandler<TextInputEvent>(this);
}

InputHandler::~InputHandler() {
    m_event_mgr.removeHandler<TextInputEvent>(this);

    setButton(Action::INTERACT, nullptr);
    setButton(Action::DELETE, nullptr);
    setButton(Action::BACKSPACE, nullptr);
    setButton(Action::NAVIGATE_RIGHT, nullptr);
    setButton(Action::NAVIGATE_LEFT, nullptr);
    setButton(Action::NAVIGATE_UP, nullptr);
    setButton(Action::NAVIGATE_DOWN, nullptr);
}

void InputHandler::setButton(Action action, InputDevice::Button* new_button) {
    InputDevice::Button*& button = m_buttons[usize(action)];

    if(button) {
        // clang-format off
        switch(action) {
            case Action::INTERACT:       button->press_signal  .unbind(this, &InputHandler::onButtonPress);
                                         button->release_signal.unbind(this, &InputHandler::onButtonRelease); break;
            case Action::DELETE:         button->press_signal  .unbind(this, &InputHandler::onDelete);        break;
            case Action::BACKSPACE:      button->press_signal  .unbind(this, &InputHandler::onBackspace);     break;
            case Action::NAVIGATE_RIGHT: button->press_signal  .unbind(this, &InputHandler::onNavigateRight); break;
            case Action::NAVIGATE_LEFT:  button->press_signal  .unbind(this, &InputHandler::onNavigateLeft);  break;
            case Action::NAVIGATE_UP:    button->press_signal  .unbind(this, &InputHandler::onNavigateUp);    break;
            case Action::NAVIGATE_DOWN:  button->press_signal  .unbind(this, &InputHandler::onNavigateDown);  break;
       break;
        }
        // clang-format on
    }
    button = new_button;
    if(button) {
        // clang-format off
        switch(action) {
            case Action::INTERACT:       button->press_signal  .bind(this, &InputHandler::onButtonPress);
                                         button->release_signal.bind(this, &InputHandler::onButtonRelease); break;
            case Action::DELETE:         button->press_signal  .bind(this, &InputHandler::onDelete);        break;
            case Action::BACKSPACE:      button->press_signal  .bind(this, &InputHandler::onBackspace);     break;
            case Action::NAVIGATE_RIGHT: button->press_signal  .bind(this, &InputHandler::onNavigateRight); break;
            case Action::NAVIGATE_LEFT:  button->press_signal  .bind(this, &InputHandler::onNavigateLeft);  break;
            case Action::NAVIGATE_UP:    button->press_signal  .bind(this, &InputHandler::onNavigateUp);    break;
            case Action::NAVIGATE_DOWN:  button->press_signal  .bind(this, &InputHandler::onNavigateDown);  break;
        break;
        }
        // clang-format on
    }
}
void InputHandler::setButtons(
    InputDevice::Button* interact,
    InputDevice::Button* del,
    InputDevice::Button* backspace,
    InputDevice::Button* navigate_right,
    InputDevice::Button* navigate_left,
    InputDevice::Button* navigate_up,
    InputDevice::Button* navigate_down
) {
    setButton(Action::INTERACT, interact);
    setButton(Action::DELETE, del);
    setButton(Action::BACKSPACE, backspace);
    setButton(Action::NAVIGATE_RIGHT, navigate_right);
    setButton(Action::NAVIGATE_LEFT, navigate_left);
    setButton(Action::NAVIGATE_UP, navigate_up);
    setButton(Action::NAVIGATE_DOWN, navigate_down);
}

void InputHandler::onCursorMoved(glm::ivec2 cursor_pos) {
    glm::ivec2 movement = cursor_pos - m_cursor_pos;
    m_cursor_pos        = cursor_pos;

    updateFocus();

    if(m_selection && m_dragging) {
        m_dragging_offset += movement;

        m_selection.handle->dragging_signal(m_dragging_offset);
    }
}

void InputHandler::onButtonPress() {
    if(m_selection != m_focus) setSelection(m_focus);

    if(!m_selection) return;

    m_selection.handle->press_signal.emit(getRelativeCursorPos(m_selection));

    m_dragging          = true;
    m_dragging_offset.x = 0;
    m_dragging_offset.y = 0;
}

void InputHandler::onButtonRelease() {
    if(!m_selection.handle || !m_dragging) return;

    m_selection.handle->release_signal.emit(m_cursor_pos);
    m_dragging = false;

    auto focus = findFocusedHndl();
    if(focus != m_focus) {
        setFocus(focus);
    } else if(m_focus == m_selection) {
        m_selection.handle->action_signal.emit(Action::INTERACT, getRelativeCursorPos(m_selection));
    } else {
        updateCursorIcon();
    }
}

void InputHandler::onDelete() {
    if(m_selection)
        m_selection.handle->action_signal.emit(Action::DELETE, getRelativeCursorPos(m_selection));
}

void InputHandler::onBackspace() {
    if(m_selection)
        m_selection.handle->action_signal.emit(
            Action::BACKSPACE, getRelativeCursorPos(m_selection)
        );
}
void InputHandler::onNavigateRight() {
    if(m_selection)
        m_selection.handle->action_signal.emit(
            Action::NAVIGATE_RIGHT, getRelativeCursorPos(m_selection)
        );
}
void InputHandler::onNavigateLeft() {
    if(m_selection)
        m_selection.handle->action_signal.emit(
            Action::NAVIGATE_LEFT, getRelativeCursorPos(m_selection)
        );
}
void InputHandler::onNavigateUp() {
    if(m_selection)
        m_selection.handle->action_signal.emit(
            Action::NAVIGATE_UP, getRelativeCursorPos(m_selection)
        );
}
void InputHandler::onNavigateDown() {
    if(m_selection)
        m_selection.handle->action_signal.emit(
            Action::NAVIGATE_DOWN, getRelativeCursorPos(m_selection)
        );
}

void InputHandler::handleEvent(In<TextInputEvent> event) {
    if(m_selection) { m_selection.handle->text_input_signal.emit(event.character); }
}

void InputHandler::updateFocus() {
    auto focus = findFocusedHndl();
    if(focus != m_focus) { setFocus(focus); }
}

InputHandler::Handle InputHandler::findFocusedHndl() {
    return findFocusedHndl(m_root_widget, glm::ivec2{0});
}

InputHandler::Handle InputHandler::findFocusedHndl(Widget& widget, glm::ivec2 const& offset) {
    const glm::ivec2 min = offset + widget.position.get();
    const glm::ivec2 max = min + widget.size.get();

    if(m_cursor_pos.x < min.x || max.x < m_cursor_pos.x || m_cursor_pos.y < min.y
       || max.y < m_cursor_pos.y)
        return {};

    // Log().info("Widget {} is in Focus", widget.getName());
    // IndentDefaultLogs indent;

    for(auto& it : widget.getChildWidgets()) {
        auto hndl = findFocusedHndl(*it, min);
        if(hndl) return hndl;
    }

    for(auto& it : widget.getInteractionHandles()) {
        if(it->isDisabled()) continue;

        const glm::ivec2 hndl_min = min + it->position;
        const glm::ivec2 hndl_max = hndl_min + it->size;

        if(hndl_min.x <= m_cursor_pos.x && m_cursor_pos.x <= hndl_max.x
           && hndl_min.y <= m_cursor_pos.y && m_cursor_pos.y <= hndl_max.y) {
            return {&widget, it};
        }
    }

    return {};
}

void InputHandler::setFocus(Handle& focus) {
    if(m_focus) m_focus.handle->setIsHovered(false);
    m_focus = focus;
    if(m_focus) m_focus.handle->setIsHovered(true);

    if(!m_dragging) updateCursorIcon();
}

void InputHandler::setSelection(Handle& selection) {
    if(m_selection) m_selection.handle->setIsSelected(false);
    m_selection = selection;
    if(m_selection) m_selection.handle->setIsSelected(true);
}

void InputHandler::updateCursorIcon() {
    auto vp = m_gui_view.getViewport();
    if(!vp) return;
    auto window_id = vp->getWindowID();

    if(m_focus.handle != nullptr) {
        kernel::CursorIcon* cursor =
            m_asset_mgr.getAsset<kernel::CursorIcon>(m_focus.handle->cursor);
        m_event_mgr.broadcast(kernel::SetCursorIconEvent{cursor, window_id});
    } else {
        m_event_mgr.broadcast(kernel::SetCursorIconEvent{nullptr, window_id});
    }
}

ivec2 InputHandler::getRelativeCursorPos(Handle& hndl) {
    ivec2 rel_pos = m_cursor_pos;
    if(hndl.handle) rel_pos -= hndl.handle->position;

    Widget* widget = hndl.widget;
    while(widget) {
        rel_pos -= widget->position.get();
        widget = widget->getParentWidget();
    }
    return rel_pos;
}

} // namespace bembel::gui
