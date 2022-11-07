module;
#include "bembel/pch.h"
//
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_IMAGE_H
module bembel.tools.font_converter;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;

FontConverter::FontConverter() {
    FT_Init_FreeType(&m_library);
}

FontConverter::~FontConverter() {}

bool FontConverter::loade(In<std::filesystem::path> path) {
    FT_Face face;

    std::string path_str = path.string();

    auto error = FT_New_Face(m_library, path_str.c_str(), 0, &face);
    if(error) {
        log().error("Failed to load TypeFace : '{}'", path_str);
        return false;
    }

    auto it = m_font_dictionary.find(face->family_name);
    if(it != m_font_dictionary.end()) {
        it->second->addFace(face);
    } else {
        auto font = std::make_unique<FontFamily>(face->family_name, face->units_per_EM);
        font->addFace(face);
        m_font_dictionary.emplace(face->family_name, font.get());
        m_fonts.push_back(std::move(font));
        font_family_added_signal.emit(face->family_name);
    }
    return true;
}

bool FontConverter::parseGlypes() {
    std::vector<char32_t> characters{
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
        's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
        'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    //for(auto const& it : m_fonts) it.second->parseGlypes(characters);

    return true;
}

/*
void Font::Save()
{
        jhl::xml::Document doc;

        jhl::xml::Element* font = doc.NewElement("Font");
        doc.InsertEndChild(font);

        jhl::xml::Element* texture = doc.NewElement("Texture");
        font->InsertEndChild(texture);
        jhl::xml::SetAttribute(texture, "file", _name + ".png");

        jhl::xml::Element* glyphs = doc.NewElement("Glyphs");
        font->InsertEndChild(glyphs);
        SaveGlypes(glyphs);

        jhl::xml::Element* charMap = doc.NewElement("CharMap");
        font->InsertEndChild(charMap);
        SaveCharMap(charMap);

        jhl::xml::Element* kerning = doc.NewElement("Kerning");
        font->InsertEndChild(kerning);
        SaveKening(kerning, _kerning);

        doc.SaveFile((_name + ".xml").c_str());
}//*/

/*
void Font::SaveGlypes(jhl::xml::Element* root) {
  float glypeScale     = 1.f / float(_unitsPerEM);
  float texCoordScaleU = _atlas->GetTexCoordScaleU();
  float texCoordScaleV = _atlas->GetTexCoordScaleV();

  for(auto& it : _glyphs) {
    jhl::xml::Element* glyph = root->GetDocument()->NewElement("Glyph");
    root->InsertEndChild(glyph);

    jhl::xml::SetAttribute(
      glyph, "advance", it.GetAdvance() / float(_unitsPerEM));

    jhl::Rectangle extends;
    if(it.GetExtendsMin() != it.GetExtendsMax()) {
      extends.max = glypeScale * glm::vec2(it.GetExtendsMax());
      extends.min = glypeScale * glm::vec2(it.GetExtendsMin());
      jhl::xml::SetAttribute(glyph, "extends", extends);
    }

    if(it.GetTexCoordMin() != it.GetTexCoordMax()) {
      jhl::Rectangle texCoord;
      texCoord.min.x = texCoordScaleU * it.GetTexCoordMin().x;
      texCoord.min.y = texCoordScaleV * it.GetTexCoordMin().y;
      texCoord.max.x = texCoordScaleU * it.GetTexCoordMax().x;
      texCoord.max.y = texCoordScaleV * it.GetTexCoordMax().y;
      jhl::xml::SetAttribute(glyph, "texCoord", texCoord);
    }
  }
}

void Font::SaveCharMap(jhl::xml::Element* root) {
  for(char32_t c : _character) {
    unsigned glypes[4] = {GetGlypeID(_faces[0], c),
                          GetGlypeID(_faces[1], c),
                          GetGlypeID(_faces[2], c),
                          GetGlypeID(_faces[3], c)};

    if(glypes[0] || glypes[1] || glypes[2] || glypes[3]) {
      jhl::xml::Element* entry = root->GetDocument()->NewElement("Entry");
      root->InsertEndChild(entry);
      jhl::xml::SetAttribute(entry, "char", c);
      if(glypes[0]) jhl::xml::SetAttribute(entry, "glyph", glypes[0]);
      if(glypes[1]) jhl::xml::SetAttribute(entry, "glyph_oblique", glypes[1]);
      if(glypes[2]) jhl::xml::SetAttribute(entry, "glyph_bold", glypes[2]);
      if(glypes[3])
        jhl::xml::SetAttribute(entry, "glyph_bold_oblique", glypes[3]);
    }
  }
}

void Font::SaveKening(jhl::xml::Element* root, const KerningMap& kerning) {
  for(auto& it : kerning) {
    jhl::xml::Element* entry = root->GetDocument()->NewElement("Entry");
    root->InsertEndChild(entry);

    jhl::xml::SetAttribute(entry, "left", it.first.first);
    jhl::xml::SetAttribute(entry, "right", it.first.second);
    jhl::xml::SetAttribute(entry, "kerning", it.second / float(_unitsPerEM));
  }
}
/*/
} // namespace bembel::tools