#ifndef BEMBEL_TOOLS_FONTCONVERTER_PREVIEWRENDERER_HPP
#define BEMBEL_TOOLS_FONTCONVERTER_PREVIEWRENDERER_HPP

#include <bembel/bembel.hpp>
#include <queue>

#include "./font-family.hpp"
//#include "GlyphTextureAtlas.h"
//#include "TextureAtlasNode.h"

class Glyph;

class PreviewRenderer : public bembel::Viewport::View {
  public:
    PreviewRenderer(FontFamily*, bembel::kernel::Texture*);
    ~PreviewRenderer();

    virtual void draw(
        const glm::ivec2& viewport_position, const glm::uvec2& viewport_size) override;

    void handleEvent(bembel::TextInputEvent& event);

  private:
    void drawGlypes();

  private:
    FontFamily*      font;
    bembel::Texture* texture;

    std::vector<char32_t> text;
};

#endif // include guards
