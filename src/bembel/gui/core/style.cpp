module;
#include <cassert>
#include <filesystem>
module bembel.gui.core;

import bembel.base;
import bembel.kernel;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

void Style::setTextureAtlas(Asset<TextureAtlas> texture_atlas) {
    m_texture_atlas = std::move(texture_atlas);
}

ColorRGBA const& Style::getColor(Colors color) const {
    assert(color != Colors::COUNT);
    return m_colors[u32(color)];
}

float Style::getValue(Values value) const {
    assert(value != Values::COUNT);
    return m_values[u64(value)];
}

Texture* Style::getMainTexture() const {
    return m_texture_atlas ? m_texture_atlas->getMainTexture() : nullptr;
}

Texture* Style::getFontTexture() const {
    return m_font ? m_font->getGlyphAtlasTexture() : nullptr;
}

TextureAtlas::Entry const* Style::getTextureCoords(std::string_view name) const {
    return m_texture_atlas ? m_texture_atlas->getEntry(name) : nullptr;
}

std::unique_ptr<Style> Style::loadAsset(std::filesystem::path file) {
    std::string const file_path = file.string(); // file.c_str() returns a wchar*
    xml::Document     doc;
    if(doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        logError("Failed to load file '{}'\n{}", file_path, doc.ErrorName());
        return nullptr;
    }

    xml::Element const* root = doc.FirstChildElement("GuiSkin");
    if(!root) {
        logError("File '{}' has no root element 'GuiSkin'", file_path);
        return nullptr;
    }
    return Style::createStyle(root);
}

std::unique_ptr<Style> Style::createAsset(xml::Element const* properties) {
    return Style::createStyle(properties);
}

std::unique_ptr<Style> Style::createStyle(xml::Element const* properties) {
    if(!properties) return nullptr;

    Asset<TextureAtlas> texture_array;
    if(!texture_array.request(properties->FirstChildElement("TextureAtlas"))) {
        logError("Can't find TextureAtlas for gui::Style");
        return nullptr;
    }
    Asset<SdfFont> font;
    if(!font.request(properties->FirstChildElement("Font"))) {
        logError("Can't find Font for gui::Style");
        return nullptr;
    }

    auto style = std::make_unique<Style>();
    style->setTextureAtlas(std::move(texture_array));
    style->setFont(std::move(font));

    for(auto widget : xml::IterateChildElements(properties, "WidgetPrototype")) {}

    return style;
}

} // namespace bembel::gui
