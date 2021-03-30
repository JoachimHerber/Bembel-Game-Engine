#include "./viewport.hpp"

namespace bembel::kernel {

Viewport::Viewport(Window* window)
: window{window} {
}

Viewport::Viewport(
    Window*          window,
    const glm::vec2& relativ_position,
    const glm::vec2& relativ_size,
    const glm::vec2& position_offset,
    const glm::vec2& size_offset)
: window{window}
, relativ_position(relativ_position)
, relativ_size(relativ_size)
, position_offset(position_offset)
, size_offset(size_offset) {
}

Viewport::~Viewport() {
    for(auto it : this->views) it->viewport = nullptr;
}

const glm::ivec2& Viewport::getPosition() const {
    return this->position;
}
const glm::uvec2& Viewport::getSize() const {
    return this->size;
}

Window* Viewport::getWindow() {
    return this->window;
}

void Viewport::enable() {
    this->enabled = true;
}

void Viewport::disable() {
    this->enabled = false;
}

bool Viewport::isEnabled() {
    return this->enabled;
}

bool Viewport::addView(View* view) {
    if(!view || view->viewport) return false;

    this->views.push_back(view);
    view->viewport = this;
    view->onResize(this->size);
    return true;
}

bool Viewport::removeView(View* view) {
    if(!view || view->viewport != this) return false;
    view->viewport = nullptr;

    auto it = std::find(this->views.begin(), this->views.end(), view);
    if(it == this->views.end()) return false;

    this->views.erase(it);
    return true;
}

void Viewport::updatePosition(const glm::vec2& frame_buffer_size) {
    this->position = glm::ivec2(frame_buffer_size * this->relativ_position + this->position_offset);
}
void Viewport::updateSize(const glm::vec2& frame_buffer_size) {
    this->size = glm::ivec2(frame_buffer_size * this->relativ_size + this->size_offset);

    for(auto view : this->views) { view->onResize(this->size); }
}

void Viewport::draw() const {
    if(!this->enabled) return;

    for(auto view : this->views) view->draw(this->position, this->size);
}

void Viewport::onCurserMove(const glm::vec2& pos) const {
    for(auto view : this->views) view->onCurserMove(pos);
}

} // namespace bembel::kernel
