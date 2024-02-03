module;
#include <memory>
module bembel.tools.font_converter;

namespace bembel::tools {
using bembel::base::ivec2;
using bembel::base::uvec2;
using bembel::kernel::TextInputEvent;
using bembel::kernel::Texture;
using bembel::kernel::Viewport;

TextureAtlasNode::TextureAtlasNode(unsigned w, unsigned h) : pos(0, 0), size(w, h) {}
TextureAtlasNode::TextureAtlasNode(unsigned x, unsigned y, unsigned w, unsigned h)
  : pos(x, y), size(w, h) {}

TextureAtlasNode::~TextureAtlasNode() {}

TextureAtlasNode* TextureAtlasNode::getNode(unsigned w, unsigned h) {
    if(this->glyph) return nullptr; // this node already contains a glyph

    if(this->size.x < w || this->size.y < h) return nullptr; // to Small

    uvec2 div = this->size - uvec2(w, h);

    if(this->child1) {
        TextureAtlasNode* node;
        if(node = this->child1->getNode(w, h)) return node;

        return this->child2->getNode(w, h);
    }

    if(div.x == 0 && div.y == 0) {
        return this; // perfect fit
    }

    if(div.x < div.y) {
        this->child1 =
            std::make_unique<TextureAtlasNode>(this->pos.x, this->pos.y, this->size.x, h);
        this->child2 = std::make_unique<TextureAtlasNode>(
            this->pos.x, this->pos.y + h, this->size.x, this->size.y - h
        );
    } else {
        this->child1 =
            std::make_unique<TextureAtlasNode>(this->pos.x, this->pos.y, w, this->size.y);
        this->child2 = std::make_unique<TextureAtlasNode>(
            this->pos.x + w, this->pos.y, this->size.x - w, this->size.y
        );
    }

    return this->child1->getNode(w, h);
}

glm::uvec2 const& TextureAtlasNode::getPos() const {
    return this->pos;
}

glm::uvec2 const& TextureAtlasNode::getSize() const {
    return this->size;
}

void TextureAtlasNode::setGlyph(Glyph* glyph) {
    this->glyph = glyph;
}

Glyph const* TextureAtlasNode::getGlyph() const {
    return this->glyph;
}

TextureAtlasNode const* TextureAtlasNode::getFirstChildNode() const {
    return this->child1.get();
}

TextureAtlasNode const* TextureAtlasNode::getSecondChildNode() const {
    return this->child2.get();
}

} // namespace bembel::tools
