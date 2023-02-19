module;
#include <filesystem>
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

FontConverter::FontConverter() : m_texture_generator{} {
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
        font_family_added_signal.emit((char8_t*)face->family_name);
    }
    return true;
}

void FontConverter::setResolution(uvec2 res) {
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

    xml::Element* texture = doc.NewElement("Texture");
    root->InsertEndChild(texture);
    texture->SetAttribute("file", (path.string() + ".png").c_str());

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

void FontConverter::converSelectedFont(
    std::vector<char32_t> characters, std::span<FontFamily::FaceType> faces, double max_dist
) {
    m_characters = std::move(characters);
    m_faces.fill(false);
    for(auto it : faces) { m_faces[std::to_underlying(it)] = true; }

    if(m_selected_font_family) {
        m_selected_font_family->parseGlypes(m_characters, faces);

        m_texture_atlas.update(m_selected_font_family->getGlyphs());
        m_texture_generator.generateTexture(*m_selected_font_family, m_texture_atlas, max_dist);
    }
}

void FontConverter::saveGlypes(xml::Element* root) {
    unsigned units_per_em      = m_selected_font_family->getUnitsPerEM();
    float    glypeScale        = 1.f / float(units_per_em);
    float    tex_coord_scale_u = m_texture_atlas.getTexCoordScaleU();
    float    tex_coord_scale_v = m_texture_atlas.getTexCoordScaleV();

    for(auto& it : m_selected_font_family->getGlyphs()) {
        xml::Element* glyph = root->GetDocument()->NewElement("Glyph");
        root->InsertEndChild(glyph);

        xml::setAttribute(glyph, "advance", it.getAdvance() / float(units_per_em));

        if(it.getExtendsMin() != it.getExtendsMax() && it.getTexCoordMin() != it.getTexCoordMax()) {
            vec2 max = glypeScale * vec2(it.getExtendsMax());
            vec2 min = glypeScale * vec2(it.getExtendsMin());
            vec4 extends{min.x, max.x, min.y, max.y};
            xml::setAttribute(glyph, "extends", extends);

            vec4 tex_coord = {
                tex_coord_scale_u * it.getTexCoordMin().x,
                tex_coord_scale_u * it.getTexCoordMax().x,
                tex_coord_scale_v * it.getTexCoordMin().y,
                tex_coord_scale_v * it.getTexCoordMax().y};
            xml::setAttribute(glyph, "texCoord", tex_coord);
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
            xml::setAttribute(entry, "char", c);
            if(m_faces[0] && glypes[0]) xml::setAttribute(entry, "glyph", glypes[0]);
            if(m_faces[1] && glypes[1]) xml::setAttribute(entry, "glyph_oblique", glypes[1]);
            if(m_faces[2] && glypes[2]) xml::setAttribute(entry, "glyph_bold", glypes[2]);
            if(m_faces[3] && glypes[3]) xml::setAttribute(entry, "glyph_bold_oblique", glypes[3]);
        }
    }
}

void FontConverter::saveKerning(xml::Element* root) {
    double scale = 1.0 / m_selected_font_family->getUnitsPerEM();
    for(auto& it : m_selected_font_family->getKerning()) {
        xml::Element* entry = root->GetDocument()->NewElement("Entry");
        root->InsertEndChild(entry);

        xml::setAttribute(entry, "left", it.first.first);
        xml::setAttribute(entry, "right", it.first.second);
        xml::setAttribute(entry, "kerning", it.second * scale);
    }
}

} // namespace bembel::tools
