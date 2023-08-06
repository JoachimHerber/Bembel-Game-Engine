module;
export module bembel.tools.font_converter:GlyphTextureAtlas;

import bembel;
import :TextureAtlasNode;
import :Glyph;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;

export class GlyphTextureAtlas {
  public:
    GlyphTextureAtlas();
    ~GlyphTextureAtlas();

    void update(std::vector<Glyph>&, uint units_per_EM);

    TextureAtlasNode const* getRoot() const { return m_root.get(); }

    void setResolution(uint res) { m_resolution = res; }

    float getTexCoordScaleU() const { return 1.f / float(m_resolution); }
    float getTexCoordScaleV() const { return 1.f / float(m_resolution); }

    Fraction getScaleFactor() const { return m_scale; }

    Signal<> update_signal;

  private:
    bool fillAtlas(std::vector<Glyph*> const&);

  private:
    uint     m_resolution = 1024;
    Fraction m_scale      = {1, 1};

    std::unique_ptr<TextureAtlasNode> m_root;
};

} // namespace bembel::tools
