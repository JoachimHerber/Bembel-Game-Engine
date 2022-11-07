module;
#include "bembel/pch.h"
export module bembel.tools.font_converter:TextureAtlasNode;

import bembel;
import :Glyph;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;

export class TextureAtlasNode {
  public:
    TextureAtlasNode(unsigned w, unsigned h);
    TextureAtlasNode(unsigned x, unsigned y, unsigned w, unsigned h);
    ~TextureAtlasNode();

    TextureAtlasNode* getNode(unsigned w, unsigned h);

    glm::uvec2 const& getPos() const;
    glm::uvec2 const& getSize() const;

    void         setGlyph(Glyph*);
    Glyph const* getGlyph() const;

    TextureAtlasNode const* getFirstChildNode() const;
    TextureAtlasNode const* getSecondChildNode() const;

  private:
    glm::uvec2 pos;
    glm::uvec2 size;

    Glyph* glyph = nullptr;

    std::unique_ptr<TextureAtlasNode> child1;
    std::unique_ptr<TextureAtlasNode> child2;
};

} // namespace bembel::tools
