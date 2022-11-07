#include "./texture-view.hpp"

namespace bembel::graphics {

TextureView::TextureView(kernel::Texture* texture) : texture{texture} {
    this->fbo->setColorAttechment(0, texture);
}

} // namespace bembel::graphics
