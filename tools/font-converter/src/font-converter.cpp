module;
#include <filesystem>

#include <memory>
#include <type_traits>
//
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_IMAGE_H
module bembel.tools.font_converter;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;

FontConverter::FontConverter() : m_texture_generator{} {
    FT_Init_FreeType(&m_library);
}

FontConverter::~FontConverter() {}

bool FontConverter::loade(In<std::filesystem::path> path) {
    FT_Face face;

    std::string path_str = path.string();

    auto error = FT_New_Face(m_library, path_str.c_str(), 0, &face);
    if(error) {
        logError("Failed to load TypeFace : '{}'", path_str);
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
        font_family_added_signal.emit((char8_t*)face->family_name);
    }
    return true;
}

void FontConverter::setResolution(uint res) {
    m_texture_atlas.setResolution(res);
    m_texture_generator.setResolution(res);
}

bool FontConverter::save(In<std::filesystem::path> path) {
    FontFamily* font = getFontFamily();
    if(!font) return false;

    if(!m_texture_generator.saveTexture(path)) {}

    bembel::base::xml::Document doc;

    xml::Element* root = doc.NewElement("Font");
    doc.InsertEndChild(root);
    root->SetAttribute("texture", std::string(path.string() + ".png").c_str());
    xml::setAttribute(root, "resolution", m_texture_generator.getResolution());
    xml::setAttribute(root, "units_per_em", m_selected_font_family->getUnitsPerEM());

    xml::Element* glyphs = doc.NewElement("Glyphs");
    root->InsertEndChild(glyphs);
    saveGlypes(glyphs);

    xml::Element* charMap = doc.NewElement("CharMap");
    root->InsertEndChild(charMap);
    saveCharMap(charMap);

    xml::Element* kerning = doc.NewElement("Kerning");
    root->InsertEndChild(kerning);
    saveKerning(kerning);

    doc.SaveFile((path.string() + ".xml").c_str());

    return true;
}

void FontConverter::converSelectedFont(std::vector<char32_t> characters, std::span<FontFamily::FaceType> faces) {
    m_characters = std::move(characters);
    m_faces.fill(false);
    for(auto it : faces) { m_faces[std::to_underlying(it)] = true; }

    if(m_selected_font_family) {
        uint max_dist = 2 * m_selected_font_family->getUnitsPerEM() / 10;

        m_selected_font_family->parseGlypes(m_characters, faces, max_dist);

        m_texture_atlas.update(m_selected_font_family->getGlyphs(), m_selected_font_family->getUnitsPerEM());
        m_texture_generator.generateTexture(
            *m_selected_font_family, m_texture_atlas, max_dist
        );
    }
}

void FontConverter::saveGlypes(xml::Element* root) {
    for(auto& it : m_selected_font_family->getGlyphs()) {
        xml::Element* glyph = root->GetDocument()->NewElement("Glyph");
        root->InsertEndChild(glyph);

        xml::setAttribute(glyph, "advance", it.getAdvance());

        if(it.getExtendsMin() != it.getExtendsMax() && it.getTexCoordMin() != it.getTexCoordMax()) {
            vec2 max = vec2(it.getExtendsMax());
            vec2 min = vec2(it.getExtendsMin());
            vec4 extends{min.x, min.y, max.x, max.y};
            xml::setAttribute(glyph, "extends", extends);

            vec4 tex_coord = {
                it.getTexCoordMin().x, it.getTexCoordMin().y, it.getTexCoordMax().x, it.getTexCoordMax().y};
            xml::setAttribute(glyph, "texCoord", tex_coord);
        }
        for(auto& sub_glyph : it.getSubGlyphs()) {
            xml::Element* sub_glyph_elem = root->GetDocument()->NewElement("SubGlyph");
            glyph->InsertEndChild(sub_glyph_elem);

            xml::setAttribute(sub_glyph_elem, "glyph", int(sub_glyph.index));
            xml::setAttribute(sub_glyph_elem, "position", sub_glyph.position);
        }
    }
}

void FontConverter::saveCharMap(xml::Element* root) {
    for(char32_t c : m_characters) {
        size_t glypes[4] = {
            m_selected_font_family->getGlypheID(c, false, false),
            m_selected_font_family->getGlypheID(c, false, true),
            m_selected_font_family->getGlypheID(c, true, false),
            m_selected_font_family->getGlypheID(c, true, true)};

        if(glypes[0] || glypes[1] || glypes[2] || glypes[3]) {
            xml::Element* entry = root->GetDocument()->NewElement("Entry");
            root->InsertEndChild(entry);
            xml::setAttribute(entry, "char", u32(c));
            if(m_faces[0] && glypes[0]) xml::setAttribute(entry, "glyph", u64(glypes[0]));
            if(m_faces[1] && glypes[1]) xml::setAttribute(entry, "glyph_oblique", u64(glypes[1]));
            if(m_faces[2] && glypes[2]) xml::setAttribute(entry, "glyph_bold", u64(glypes[2]));
            if(m_faces[3] && glypes[3]) xml::setAttribute(entry, "glyph_bold_oblique", u64(glypes[3]));
        }
    }
}

void FontConverter::saveKerning(xml::Element* root) {
    double scale = 1.0 / m_selected_font_family->getUnitsPerEM();
    for(auto& it : m_selected_font_family->getKerning()) {
        xml::Element* entry = root->GetDocument()->NewElement("Entry");
        root->InsertEndChild(entry);

        xml::setAttribute(entry, "left", u32(it.first.first));
        xml::setAttribute(entry, "right", u32(it.first.second));
        xml::setAttribute(entry, "kerning", it.second * scale);
    }
}

} // namespace bembel::tools
