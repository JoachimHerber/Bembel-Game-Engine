module;
#include "bembel/pch.h"
module bembel.tools.font_converter;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;

GlyphTextureAtlas::GlyphTextureAtlas() {}

GlyphTextureAtlas::~GlyphTextureAtlas() {}

void GlyphTextureAtlas::update(std::vector<Glyph>& glyphs) {
    std::vector<Glyph*> glyph_ptrs;

    double reqiredArea = 0;
    for(auto& glyph : glyphs) {
        glyph.setTexCoordMin({0, 0});
        glyph.setTexCoordMax({0, 0});

        ivec2 size = glyph.getSize();

        if(!glyph.getOutline().empty()) {
            reqiredArea += (size.x) * (size.y);
            glyph_ptrs.push_back(&glyph);
        } else {
            glyph.setTexCoordMin(ivec2(0));
            glyph.setTexCoordMax(ivec2(0));
        }
    }

    std::sort(glyph_ptrs.begin(), glyph_ptrs.end(), [](Glyph const* g1, Glyph const* g2) {
        return g1->getSize().x * g1->getSize().y > g2->getSize().x * g2->getSize().y;
    });

    m_scale      = .99 / sqrt(reqiredArea);
    bool success = false;
    while(!success) {
        // try to fill texture atlas
        m_root  = std::make_unique<TextureAtlasNode>(m_resolution.x, m_resolution.y);
        success = fillAtlas(glyph_ptrs);

        if(!success) {
            // scale down glyphs until they fit
            m_scale *= .95;
        }
    }

    this->update_signal.emit();
}

bool GlyphTextureAtlas::fillAtlas(std::vector<Glyph*> const& glypes) {
    for(auto& glyph : glypes) {
        ivec2  size = glyph->getSize();
        double w, h;

        w         = m_resolution.x * m_scale * (size.x);
        h         = m_resolution.y * m_scale * (size.y);
        auto node = m_root->getNode(uint(floor(w) + 1), uint(floor(h) + 1));

        if(node) {
            glyph->setTexCoordMin(node->getPos());
            glyph->setTexCoordMax(node->getPos() + node->getSize());
            node->setGlyph(glyph);
        } else
            return false;
    }

    return true;
}

} // namespace bembel::tools