#ifndef BEMBEL_TOOLS_FONTCONVERTER_GLYPH_TEXTUREA_TLAS_HPP
#define BEMBEL_TOOLS_FONTCONVERTER_GLYPH_TEXTUREA_TLAS_HPP

#include <bembel/bembel.hpp>

#include "texture-atlas-node.hpp"

class GlyphTextureAtlas {
  public:
    GlyphTextureAtlas();
    ~GlyphTextureAtlas();

    void update(std::vector<Glyph>&);

    const TextureAtlasNode* getRoot() const;

    void setResolution(unsigned w, unsigned h);

    float getTexCoordScaleU() const;
    float getTexCoordScaleV() const;

    double getScaleFactor() const;

    bembel::Signal<>& getUpdateSignal();

  private:
    bool fillAtlas(const std::vector<Glyph*>&);

  private:
    glm::uvec2 resolution;

    double scale;

    std::unique_ptr<TextureAtlasNode> root;

    bembel::Signal<> updateSignal;
};

#endif // include guards
