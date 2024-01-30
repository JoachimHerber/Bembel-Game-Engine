module;
export module bembel.tools.font_converter:FontView;

import bembel;
import :FontFamily;
import :GlyphTextureAtlas;
import :FontTextureGenerator;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;

export class FontView : public Viewport::View {
  public:
    FontView(GlyphTextureAtlas&, FontTextureGenerator&);
    ~FontView();

    void setFont(FontFamily*);

    virtual void draw(In<ivec2> viewport_position, In<uvec2> viewport_size) override;

  private:
    GlyphTextureAtlas&    m_texture_atlas;
    FontTextureGenerator& m_texture_gen;

    bool m_requires_update = true;

    FontFamily* m_font = nullptr;

    double m_line_width = 4.0f;

    Asset<ShaderProgram> m_shader;
};

} // namespace bembel::tools
