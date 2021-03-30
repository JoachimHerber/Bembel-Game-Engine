#include "./texture-view.hpp"

namespace bembel::graphics {

TextureView::TextureView(kernel::Texture* texture)
: texture{texture}
, view_area_pos{0, 0}
, view_area_size{1, 1}
, fbo{std::make_unique<kernel::FrameBufferObject>()} {
    this->fbo->setColorAttechment(0, texture);
}

TextureView::~TextureView() {
}

void TextureView::setViewArea(const glm::ivec2& pos, const glm::uvec2& size) {
    this->view_area_pos  = pos;
    this->view_area_size = size;
}

glm::ivec2 TextureView::getViewAreaPosition() const {
    return this->view_area_pos;
}

glm::uvec2 TextureView::getViewAreaSize() const {
    return this->view_area_size;
}

void TextureView::init() {
    this->fbo->init();
}

void TextureView::cleanup() {
    this->fbo->cleanup();
}

void TextureView::draw(const glm::ivec2& viewport_position, const glm::uvec2& viewport_size) {
    this->fbo->blitToBackBuffer(
        this->view_area_pos,
        this->view_area_pos + glm::ivec2(this->view_area_size),
        viewport_position,
        viewport_position + glm::ivec2(viewport_size));
}

} // namespace bembel::graphics
