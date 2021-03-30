#include "./font-family.hpp"

#include <limits>

FontFamily::FontFamily(const std::string& name, unsigned int units_per_EM)
: name{name}
, units_per_EM{units_per_EM} {
}

bool FontFamily::addFace(const FT_Face& face) {
    FT_Set_Pixel_Sizes(face, face->units_per_EM / 64, face->units_per_EM / 64);

    // test if the face matches previews loaded faces
    if(face->units_per_EM != this->units_per_EM) return false; // unitsPerEM do not match

    int index = face->style_flags & 0b0011;

    if(this->faces[index]) return false; // face with same flags already loaded

    this->faces[index] = std::make_unique<Face>(face);

    return true;
}

bool FontFamily::parseGlypes(const std::vector<char32_t>& characters) {
    this->glyphs.clear();
    this->kerning.clear();

    if(!this->faces[0]) return false;

    // init '.notdef' glyph
    this->glyphs.push_back(Glyph());
    this->glyphs.back().init(this->faces[0]->face, 0, 2 * this->units_per_EM / 10);

    for(int i = 0; i < 4; ++i) {
        if(!this->faces[i]) continue;
        FT_Face  face    = this->faces[i]->face;
        CharMap& charMap = this->faces[i]->charMap;

        std::map<unsigned, int> glyphMap;
        for(char32_t c : characters) {
            unsigned glyph_index = FT_Get_Char_Index(face, c);
            if(glyph_index == 0) continue; //'.notdef' glyph

            auto it = glyphMap.find(glyph_index);
            if(it != glyphMap.end()) {
                this->faces[i]->charMap.emplace(c, it->second);
            } else {
                glyphMap.emplace(glyph_index, this->glyphs.size());
                charMap.emplace(c, this->glyphs.size());
                this->glyphs.push_back(Glyph());
                this->glyphs.back().init(face, glyph_index, 2 * this->units_per_EM / 10);
            }
        }

        for(auto left : glyphMap) {
            for(auto right : glyphMap) {
                FT_Vector kerning;
                FT_Get_Kerning(face, left.first, right.first, FT_KERNING_UNSCALED, &kerning);
                if(kerning.x != 0) {
                    auto key = std::make_pair(left.second, right.second);
                    this->kerning.emplace(key, kerning.x);
                }
            }
        }
    }

    this->texture_atlas.update(this->glyphs);
    return true;
}

size_t FontFamily::getGlypheID(char32_t c, bool bold, bool oblique) {
    auto& face = this->faces[(bold ? 2 : 0) | (oblique ? 1 : 0)];
    auto  it   = face->charMap.find(c);
    if(it != face->charMap.end()) return it->second;

    return std::numeric_limits<size_t>::max();
}

std::vector<Glyph>& FontFamily::getGlyphs() {
    return this->glyphs;
}

const std::vector<Glyph>& FontFamily::getGlyphs() const {
    return this->glyphs;
}

const std::string& FontFamily::getName() {
    return this->name;
}

unsigned FontFamily::getUnitsPerEM() {
    return this->units_per_EM;
}

GlyphTextureAtlas& FontFamily::getTextureAtlas() {
    return this->texture_atlas;
}
