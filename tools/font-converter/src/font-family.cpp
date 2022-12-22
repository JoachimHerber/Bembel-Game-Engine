module;
#include <memory>
//
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_IMAGE_H
module bembel.tools.font_converter;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;

FontFamily::FontFamily(std::string_view name, unsigned int units_per_EM)
  : m_name{name}, m_units_per_EM{units_per_EM} {}

bool FontFamily::addFace(FT_Face const& face) {
    FT_Set_Pixel_Sizes(face, face->units_per_EM / 64, face->units_per_EM / 64);

    // test if the face matches previews loaded faces
    if(face->units_per_EM != m_units_per_EM) return false; // unitsPerEM do not match

    int index = face->style_flags & 0b0011;

    if(m_faces[index]) return false; // face with same flags already loaded

    m_faces[index] = std::make_unique<Face>(face);

    return true;
}

bool FontFamily::parseGlypes(std::span<char32_t> characters, std::span<FaceType> faces) {
    m_glyphs.clear();
    m_kerning.clear();

    if(!m_faces[0]) return false;

    // init '.notdef' glyph
    m_glyphs.emplace_back();
    m_glyphs.back().init(m_faces[0]->face, 0, 2 * m_units_per_EM / 10);

    for(auto i : faces) {
        if(!m_faces[i]) continue;
        FT_Face  face    = m_faces[i]->face;
        CharMap& charMap = m_faces[i]->charMap;

        std::map<unsigned, int> glyphMap;
        for(char32_t c : characters) {
            unsigned glyph_index = FT_Get_Char_Index(face, c);
            if(glyph_index == 0) continue; //'.notdef' glyph

            auto it = glyphMap.find(glyph_index);
            if(it != glyphMap.end()) {
                m_faces[i]->charMap.emplace(c, it->second);
            } else {
                glyphMap.emplace(glyph_index, m_glyphs.size());
                charMap.emplace(c, m_glyphs.size());
                m_glyphs.emplace_back();
                m_glyphs.back().init(face, glyph_index, 2 * m_units_per_EM / 10);
            }
        }

        for(auto left : glyphMap) {
            for(auto right : glyphMap) {
                FT_Vector kerning;
                FT_Get_Kerning(face, left.first, right.first, FT_KERNING_UNSCALED, &kerning);
                if(kerning.x != 0) {
                    auto key = std::make_pair(left.second, right.second);
                    m_kerning.emplace(key, float(kerning.x));
                }
            }
        }
    }
    return true;
}

size_t FontFamily::getGlypheID(char32_t c, bool bold, bool oblique) {
    auto& face = m_faces[(bold ? 2 : 0) | (oblique ? 1 : 0)];
    if(face) {
        auto it = face->charMap.find(c);
        if(it != face->charMap.end()) return it->second;
    }

    return 0;
}

} // namespace bembel::tools
