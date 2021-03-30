#include "./interaction-handle.hpp"

namespace bembel::gui {

InteractionHandle::InteractionHandle(Widget* widget)
: widget{widget}
, state{State::IDLE} {
}

InteractionHandle::~InteractionHandle() {
}

Widget* InteractionHandle::getWidget() const {
    return this->widget;
}

const glm::ivec2& InteractionHandle::getPosition() const {
    return this->position;
}

void InteractionHandle::setPosition(const glm::ivec2& pos) {
    this->position = pos;
}

void InteractionHandle::setPosition(int x, int y) {
    this->position = glm::ivec2{x, y};
}

const glm::ivec2& InteractionHandle::getSize() const {
    return this->size;
}

void InteractionHandle::setSize(const glm::ivec2& size) {
    this->size = size;
}

void InteractionHandle::setSize(int w, int h) {
    this->size = glm::ivec2{w, h};
}

InteractionHandle::State InteractionHandle::getState() const {
    return this->state;
}

bool InteractionHandle::isEnabled() const {
    return this->state != State::DISABLED;
}

bool InteractionHandle::isSelected() const {
    return this->state == State::SELECTED || this->state == State::ACTIVE;
}

bool InteractionHandle::isActive() const {
    return this->state == State::ACTIVE;
}

void InteractionHandle::disable() {
    setState(State::DISABLED);
}

void InteractionHandle::enable() {
    if(this->state == State::DISABLED) setState(State::IDLE);
}

kernel::AssetHandle InteractionHandle::getCursor() {
    return this->cursor;
}

void InteractionHandle::setCursor(kernel::AssetHandle cursor) {
    this->cursor = cursor;
}

void InteractionHandle::setState(State new_state) {
    if(new_state == this->state) return;

    this->state_change_signal.emit(this->state, new_state);
    this->state = new_state;
}

} // namespace bembel::gui
