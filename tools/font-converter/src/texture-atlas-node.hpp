#ifndef BEMBEL_TOOLS_FONTCONVERTER_TEXTURE_ATLAS_NODE_HPP
#define BEMBEL_TOOLS_FONTCONVERTER_TEXTURE_ATLAS_NODE_HPP

#include <memory>

#include "glyph.hpp"

class TextureAtlasNode {
 public:
  TextureAtlasNode(unsigned w, unsigned h);
  TextureAtlasNode(unsigned x, unsigned y, unsigned w, unsigned h);
  ~TextureAtlasNode();

  TextureAtlasNode* getNode(unsigned w, unsigned h);

  const glm::uvec2& getPos() const;
  const glm::uvec2& getSize() const;

  void setGlyph(Glyph*);
  const Glyph* getGlyph() const;

  const TextureAtlasNode* getFirstChildNode() const;
  const TextureAtlasNode* getSecondChildNode() const;

 private:
  glm::uvec2 pos;
  glm::uvec2 size;

  Glyph* glyph = nullptr;

  std::unique_ptr<TextureAtlasNode> child1;
  std::unique_ptr<TextureAtlasNode> child2;
};

#endif // include guards
