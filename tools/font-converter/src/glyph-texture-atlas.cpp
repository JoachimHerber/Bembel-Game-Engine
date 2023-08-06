module;
#include <algorithm>
#include <glm/glm.hpp>
#include <memory>
#include <string_view>
#include <valarray>
#include <vector>
module bembel.tools.font_converter;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;

GlyphTextureAtlas::GlyphTextureAtlas() {}

GlyphTextureAtlas::~GlyphTextureAtlas() {}

Fraction roundScale(double scale) {
    double frat = scale - std::floor(scale);

    uint denominator = std::floor(1 / frat);
    int numerator   = denominator * int(std::floor(scale)) + 1;

    log().info("Scale = {}/{}", numerator, denominator);
    return {numerator, denominator};
}

void GlyphTextureAtlas::update(std::vector<Glyph>& glyphs, uint units_per_EM) {
    std::vector<Glyph*> glyph_ptrs;

    double reqiredArea = 0;
    for(auto& glyph : glyphs) {
        glyph.setTexCoordMin({0, 0});
        glyph.setTexCoordMax({0, 0});

        ivec2 size = glyph.getSize();

        if(!glyph.getOutline().empty()) {
            reqiredArea += size.x * size.y;
            glyph_ptrs.push_back(&glyph);
        }
    }

    std::sort(glyph_ptrs.begin(), glyph_ptrs.end(), [](Glyph const* g1, Glyph const* g2) {
        return g1->getSize().x * g1->getSize().y > g2->getSize().x * g2->getSize().y;
    });
    double scale   = 0.95 * m_resolution / sqrt(reqiredArea);
    bool   success = false;
    while(!success) {
        // try to fill texture atlas
        m_scale = roundScale(scale);
        m_root  = std::make_unique<TextureAtlasNode>(m_resolution, m_resolution);
        success = fillAtlas(glyph_ptrs);

        if(!success) {
            // scale down glyphs until they fit
            scale *= 0.95;
        }
    }
    log().info("GlyphTextureAtlas filled successfully: scale = {}/{}", m_scale.numerator, m_scale.denominator);

    this->update_signal.emit();
}

bool GlyphTextureAtlas::fillAtlas(std::vector<Glyph*> const& glypes) {
    for(auto& glyph : glypes) {
        ivec2 size = glyph->getSize();

        if(size.x % m_scale.denominator != 0) { size.x += m_scale.denominator - (size.x % m_scale.denominator); }
        if(size.y % m_scale.denominator != 0) { size.y += m_scale.denominator - (size.y % m_scale.denominator); }

        int w = m_scale.numerator * size.x / m_scale.denominator;
        int h = m_scale.numerator * size.y / m_scale.denominator;

        auto node = m_root->getNode(w, h);

        if(node) {
            glyph->setTexCoordMin(node->getPos());
            glyph->setTexCoordMax(node->getPos() + node->getSize());

            glyph->setExtendsMax(glyph->getExtendsMin() + size);
            node->setGlyph(glyph);
        } else
            return false;
    }

    return true;
}

} // namespace bembel::tools
