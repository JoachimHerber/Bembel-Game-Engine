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
    FontView(AssetManager&, GlyphTextureAtlas&, FontTextureGenerator&);
    ~FontView();

    void setFont(FontFamily*);

    virtual void draw(ivec2 const& viewport_position, uvec2 const& viewport_size) override;

  private:
    AssetManager&         m_asset_mgr;
    GlyphTextureAtlas&    m_texture_atlas;
    FontTextureGenerator& m_texture_gen;

    bool m_requires_update = true;

    FontFamily* m_font = nullptr;

    double m_line_width = 4.0f;

    Asset<ShaderProgram> m_shader;
};

} // namespace bembel::tools
