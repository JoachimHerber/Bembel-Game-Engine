module;
#include "bembel/pch.h"
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

    void update(std::vector<Glyph>&);

    TextureAtlasNode const* getRoot() const { return m_root.get(); }

    void setResolution(uvec2 res) { m_resolution = res; }

    float getTexCoordScaleU() const { return 1.f / float(m_resolution.x); }
    float getTexCoordScaleV() const { return 1.f / float(m_resolution.y); }

    double getScaleFactor() const { return m_scale; }

    Signal<> update_signal;

  private:
    bool fillAtlas(std::vector<Glyph*> const&);

  private:
    uvec2  m_resolution = {1024, 1024};
    double m_scale      = 1.0;

    std::unique_ptr<TextureAtlasNode> m_root;
};

} // namespace bembel::tools
