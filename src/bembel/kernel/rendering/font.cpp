#include "Font.hpp"

namespace bembel::kernel {

Font::Font() {
}

Font::~Font() {
}

float Font::getAscender() const {
    return this->ascender;
}

float Font::getDescender() const {
    return this->descender;
}

Texture* Font::getGlyphAtlasTexture() const {
    return this->glyph_atlas_texture.get();
}

unsigned Font::getGlyphIndex(char32_t character, bool bold, bool oblique) const {
    const auto& char_map = this->char_maps
                               [bold ? (oblique ? CharMapIndex::BoldOblique : CharMapIndex::Bold)
                                     : (oblique ? CharMapIndex::Oblique : CharMapIndex::Default)];

    auto glyph = char_map.find(character);
    if(glyph == char_map.end()) {
        return INVALIDE_GLYPH_INDEX;
    } else {
        return glyph->second;
    }
}

float Font::getAdvance(unsigned glyph_index) const {
    if(glyph_index >= this->glypths.size()) return 0.0f;

    return this->glypths[glyph_index].advance;
}

float Font::getAdvance(std::vector<unsigned>& glyph_indices) const {
    float    advance    = 0.0f;
    unsigned prev_glyph = 0;
    for(unsigned glyph : glyph_indices) {
        if(glyph >= this->glypths.size()) continue;

        advance += this->glypths[glyph].advance;
        advance += this->getKernig(prev_glyph, glyph);
        prev_glyph = glyph;
    }
    return advance;
}

float Font::getKernig(unsigned left_glyph_index, unsigned right_glyph_index) const {
    auto key = std::make_pair(left_glyph_index, right_glyph_index);
    auto it  = this->kernig.find(key);
    if(it != this->kernig.end()) { return it->second; }

    return 0.0f;
}

const Font::Glyph& Font::getGlypData(unsigned glyph_index) const {
    if(glyph_index < this->glypths.size()) return this->glypths[glyph_index];

    const static Glyph unknow_glyph{
        0.f,
        {0.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 0.0f},
    };

    return unknow_glyph;
}

unsigned Font::getNumGlyphs() const {
    return this->glypths.size();
}

const std::string& Font::getTypeName() {
    const static std::string type_name = "Font";
    return type_name;
}

std::unique_ptr<Font> Font::loadAsset(AssetManager& asset_mgr, const std::string& file_name) {
    xml::Document doc;

    doc.LoadFile(file_name.c_str());
    if(doc.Error()) {
        BEMBEL_LOG_ERROR() << "Can't load file \"" << file_name << "\"\n"
                           << doc.GetErrorStr1() << std::endl;
        return nullptr;
    }

    xml::Element* root = doc.FirstChildElement("Font");
    if(doc.Error()) {
        BEMBEL_LOG_ERROR() << "\"" << file_name << "\" has no root element \"Font\"" << std::endl;
        return nullptr;
    }
    return createAsset(asset_mgr, root);
}

std::unique_ptr<Font> Font::createAsset(AssetManager& asset_mgr, const xml::Element* properties) {
    auto font = std::make_unique<Font>();

    std::string texture;
    if(!xml::getAttribute(properties, "texture", texture)) { return nullptr; }

    font->glyph_atlas_texture.request(asset_mgr, texture);
    if(!font->glyph_atlas_texture) return nullptr;

    if(!font->readGlyphs(properties->FirstChildElement("Glyphs"))) {
        BEMBEL_LOG_ERROR() << "Can't parse glyph informations" << std::endl;
        return nullptr;
    }

    if(!font->readCharMap(properties->FirstChildElement("CharMap"))) {
        BEMBEL_LOG_ERROR() << "Can't parse char-map informations" << std::endl;
        return nullptr;
    }

    if(!font->readKerning(properties->FirstChildElement("Kerning"))) {
        BEMBEL_LOG_ERROR() << "Can't parse kerning informations" << std::endl;
        return nullptr;
    }

    // compute ascender and descender
    for(auto it : font->glypths) {
        font->ascender  = glm::max(font->ascender, it.extents_max.y);
        font->descender = glm::min(font->descender, it.extents_min.y);
    }

    return font;
}

void Font::deleteAsset(AssetManager&, std::unique_ptr<Font>) {
}

bool Font::readGlyphs(const xml::Element* properties) {
    if(!properties) return false;

    for(auto glyphProps : xml::IterateChildElements(properties, "Glyph")) {
        Glyph glyph;
        if(!xml::getAttribute(glyphProps, "advance", glyph.advance)) return false;

        glm::vec4 tmp;
        if(xml::getAttribute(glyphProps, "extends", tmp)) {
            glyph.extents_min = {tmp.x, tmp.z};
            glyph.extents_max = {tmp.y, tmp.w};
        } else {
            return false;
        }
        if(xml::getAttribute(glyphProps, "texCoord", tmp)) {
            glyph.tex_coords_min = {tmp.x, tmp.z};
            glyph.tex_coords_max = {tmp.y, tmp.w};
        }
        this->glypths.push_back(glyph);
    }
    return true;
}

bool Font::readCharMap(const xml::Element* properties) {
    if(!properties) return false;

    for(auto entry : xml::IterateChildElements(properties, "Entry")) {
        unsigned c, glyph, glyph_oblique, glyph_bold, glyph_bold_oblique;

        if(!xml::getAttribute(entry, "char", c)) return false;
        if(!xml::getAttribute(entry, "glyph", glyph)) return false;
        if(!xml::getAttribute(entry, "glyph_oblique", glyph_oblique)) return false;
        if(!xml::getAttribute(entry, "glyph_bold", glyph_bold)) return false;
        if(!xml::getAttribute(entry, "glyph_bold_oblique", glyph_bold_oblique)) return false;

        this->char_maps[CharMapIndex::Default][c]     = glyph;
        this->char_maps[CharMapIndex::Oblique][c]     = glyph_oblique;
        this->char_maps[CharMapIndex::Bold][c]        = glyph_bold;
        this->char_maps[CharMapIndex::BoldOblique][c] = glyph_bold_oblique;
    }
    return true;
}

bool Font::readKerning(const xml::Element* properties) {
    if(!properties) return false;

    for(auto entry : xml::IterateChildElements(properties, "Entry")) {
        unsigned left, right;
        float    kerning;

        if(!xml::getAttribute(entry, "left", left)) return false;
        if(!xml::getAttribute(entry, "right", right)) return false;
        if(!xml::getAttribute(entry, "kerning", kerning)) return false;

        auto key          = std::make_pair(left, right);
        this->kernig[key] = kerning;
    }
    return true;
}

} // namespace bembel::kernel
