module;
#include <memory>
#include <string>
#include <string_view>
#include <utility>
module bembel.kernel.rendering;

import bembel.base;

namespace bembel::kernel {
using namespace bembel::base;

GlyphIndex Font::getGlyphIndex(char32_t character, bool bold, bool oblique) const {
    auto const& char_map = m_char_maps
        [bold ? (oblique ? CharMapIndex::BoldOblique : CharMapIndex::Bold)
              : (oblique ? CharMapIndex::Oblique : CharMapIndex::Default)];

    auto glyph = char_map.find(character);
    if(glyph == char_map.end()) {
        return INVALIDE_GLYPH_INDEX;
    } else {
        return glyph->second;
    }
}

float Font::getAdvance(GlyphIndex glyph_index) const {
    if(glyph_index >= m_glypths.size()) return 0.0f;

    return m_glypths[glyph_index].advance;
}

float Font::getAdvance(std::span<GlyphIndex> glyph_indices) const {
    float    advance    = 0.0f;
    unsigned prev_glyph = 0;
    for(unsigned glyph : glyph_indices) {
        if(glyph >= m_glypths.size()) continue;

        advance += m_glypths[glyph].advance;
        advance += getKernig(prev_glyph, glyph);
        prev_glyph = glyph;
    }
    return advance;
}

float Font::getKernig(GlyphIndex left, GlyphIndex right) const {
    auto key = std::make_pair(left, right);
    auto it  = m_kernig.find(key);
    if(it != m_kernig.end()) { return it->second; }

    return 0.0f;
}

Font::Glyph const& Font::getGlypData(unsigned glyph_index) const {
    if(glyph_index < m_glypths.size()) return m_glypths[glyph_index];

    static constexpr Glyph unknow_glyph{
        0.f,
        {0.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 0.0f},
    };

    return unknow_glyph;
}

std::unique_ptr<Font> Font::loadAsset(std::filesystem::path file) {
    std::string const file_path = file.string(); // file.c_str() returns a wchar*
    xml::Document     doc;
    if(doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        log().error("Can't load file '{}'\n'{}'", file_path, doc.ErrorStr());
        return nullptr;
    }

    xml::Element* root = doc.FirstChildElement("Font");
    if(!root) {
        log().error("'{}' has no root element 'Font'", file_path);
        return nullptr;
    }
    return createAsset(root);
}

std::unique_ptr<Font> Font::createAsset(xml::Element const* properties) {
    auto font = std::make_unique<Font>();

    std::string texture;
    if(!xml::getAttribute(properties, "texture", texture)) { return nullptr; }

    font->m_glyph_atlas_texture.request(texture);
    if(!font->m_glyph_atlas_texture) {
        log().error("Can't find texture for font");
        return nullptr;
    }

    if(!font->readGlyphs(properties->FirstChildElement("Glyphs"))) {
        log().error("Can't parse glyph informations");
        return nullptr;
    }

    if(!font->readCharMap(properties->FirstChildElement("CharMap"))) {
        log().error("Can't parse char-map informations");
        return nullptr;
    }

    if(!font->readKerning(properties->FirstChildElement("Kerning"))) {
        log().error("Can't parse kerning informations");
        return nullptr;
    }

    // compute ascender and descender
    for(auto it : font->m_glypths) {
        font->m_ascender  = max(font->m_ascender, it.extents_max.y);
        font->m_descender = min(font->m_descender, it.extents_min.y);
    }

    return font;
}

bool Font::readGlyphs(xml::Element const* properties) {
    if(!properties) return false;

    for(auto glyphProps : xml::IterateChildElements(properties, "Glyph")) {
        Glyph glyph;
        if(!xml::getAttribute(glyphProps, "advance", glyph.advance)) return false;

        vec4 tmp;
        if(xml::getAttribute(glyphProps, "extends", tmp)) {
            glyph.extents_min = {tmp.x, tmp.z};
            glyph.extents_max = {tmp.y, tmp.w};
        } else {
            return false;
        }
        if(base::xml::getAttribute(glyphProps, "texCoord", tmp)) {
            glyph.tex_coords_min = {tmp.x, tmp.z};
            glyph.tex_coords_max = {tmp.y, tmp.w};
        }
        this->m_glypths.push_back(glyph);
    }
    return true;
}

bool Font::readCharMap(base::xml::Element const* properties) {
    if(!properties) return false;

    for(auto entry : xml::IterateChildElements(properties, "Entry")) {
        unsigned c, glyph, glyph_oblique, glyph_bold, glyph_bold_oblique;

        if(!xml::getAttribute(entry, "char", c)) return false;
        if(!xml::getAttribute(entry, "glyph", glyph)) return false;
        if(!xml::getAttribute(entry, "glyph_oblique", glyph_oblique)) return false;
        if(!xml::getAttribute(entry, "glyph_bold", glyph_bold)) return false;
        if(!xml::getAttribute(entry, "glyph_bold_oblique", glyph_bold_oblique)) return false;

        m_char_maps[CharMapIndex::Default][c]     = glyph;
        m_char_maps[CharMapIndex::Oblique][c]     = glyph_oblique;
        m_char_maps[CharMapIndex::Bold][c]        = glyph_bold;
        m_char_maps[CharMapIndex::BoldOblique][c] = glyph_bold_oblique;
    }
    return true;
}

bool Font::readKerning(xml::Element const* properties) {
    if(!properties) return false;

    for(auto entry : xml::IterateChildElements(properties, "Entry")) {
        unsigned left, right;
        float    kerning;

        if(!xml::getAttribute(entry, "left", left)) return false;
        if(!xml::getAttribute(entry, "right", right)) return false;
        if(!xml::getAttribute(entry, "kerning", kerning)) return false;

        auto key            = std::make_pair(left, right);
        this->m_kernig[key] = kerning;
    }
    return true;
}

} // namespace bembel::kernel
