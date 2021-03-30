#include "./style.hpp"

#include <bembel/kernel/assets/asset-manager.hpp>
#include <bembel/kernel/display/window.hpp>
#include <bembel/kernel/rendering/shader.hpp>
#include <bembel/kernel/rendering/texture.hpp>

namespace bembel::gui {

Style::Style() {
    this->colors.resize(unsigned(Colors::COUNT));

    this->colors[unsigned(Colors::TEXT)]                       = {0xff, 0xff, 0xff, 0xff};
    this->colors[unsigned(Colors::TEXT_OUTLINE)]               = {0x00, 0x00, 0x00, 0xff};
    this->colors[unsigned(Colors::TEXT_DISABLED)]              = {0x80, 0x80, 0x80, 0xff};
    this->colors[unsigned(Colors::WINDOW_BACKGROUND)]          = {0x0f, 0x0f, 0x0f, 0xf0};
    this->colors[unsigned(Colors::WINDOW_BORDER)]              = {0x0c, 0x0c, 0x0c, 0xff};
    this->colors[unsigned(Colors::MENU_BAR_BACKGROUND)]        = {0x24, 0x24, 0x24, 0xff};
    this->colors[unsigned(Colors::BUTTON_BACKGROUND)]          = {0x42, 0x96, 0xfa, 0x66};
    this->colors[unsigned(Colors::BUTTON_BACKGROUND_DISABLED)] = {0x80, 0x80, 0x80, 0x66};
    this->colors[unsigned(Colors::BUTTON_BACKGROUND_HOVERED)]  = {0x42, 0x96, 0xfa, 0xff};
    this->colors[unsigned(Colors::BUTTON_BACKGROUND_ACTIVE)]   = {0x0f, 0x87, 0xfa, 0xff};
    this->colors[unsigned(Colors::BUTTON_BORDER)]              = {0x35, 0x78, 0xc8, 0x66};
    this->colors[unsigned(Colors::BUTTON_BORDER_DISABLED)]     = {0x66, 0x66, 0x66, 0x66};
    this->colors[unsigned(Colors::BUTTON_BORDER_HOVERED)]      = {0x35, 0x78, 0xc8, 0xff};
    this->colors[unsigned(Colors::BUTTON_BORDER_ACTIVE)]       = {0x0c, 0x6c, 0xc8, 0xff};

    this->values.resize(unsigned(Values::COUNT));
    this->values[unsigned(Values::MIN_FONT_SIZE)]           = 8;
    this->values[unsigned(Values::WINDOW_BORDER_WIDTH)]     = 4;
    this->values[unsigned(Values::WINDOW_TITLE_BAR_HEIGHT)] = 32;
    this->values[unsigned(Values::BUTTON_BORDER_WIDTH)]     = 4;
}

Style::~Style() {
}

void Style::setFont(kernel::Asset<kernel::Font> font) {
    this->font = font;
}

kernel::Font* Style::getFont() {
    return this->font.get();
}

void Style::setTextureAtlas(kernel::Asset<kernel::TextureAtlas> texture_atlas) {
    this->texture_atlas = texture_atlas;
}

const glm::tvec4<uint8_t>& Style::getColor(Colors color) {
    assert(color != Colors::COUNT);
    return this->colors[unsigned(color)];
}

float Style::getValue(Values value) {
    assert(value != Values::COUNT);
    return this->values[unsigned(value)];
}

kernel::Texture* Style::getMainTexture() const {
    return this->texture_atlas ? this->texture_atlas->getMainTexture() : nullptr;
}

kernel::Texture* Style::getFontTexture() const {
    return this->font ? this->font->getGlyphAtlasTexture() : nullptr;
}

const std::pair<glm::vec2, glm::vec2>* Style::getTextureCoords(const std::string& name) const {
    return this->texture_atlas ? this->texture_atlas->getEnty(name) : nullptr;
}

const std::string& Style::getTypeName() {
    static const std::string type_name = "GuiStyle";
    return type_name;
}

std::unique_ptr<Style> Style::loadAsset(
    kernel::AssetManager& asset_mgr, const std::string& file_name) {
    base::xml::Document doc;
    if(doc.LoadFile(file_name.c_str()) != tinyxml2::XML_SUCCESS) {
        BEMBEL_LOG_ERROR() << "Failed to load file '" << file_name << "'\n"
                           << doc.ErrorName() << std::endl;
        return nullptr;
    }

    const base::xml::Element* root = doc.FirstChildElement("GuiSkin");
    if(!root) {
        BEMBEL_LOG_ERROR() << "File '" << file_name << "' has no root element 'GuiSkin'"
                           << std::endl;
        return nullptr;
    }
    return Style::createStyle(asset_mgr, root);
}

std::unique_ptr<Style> Style::createAsset(
    kernel::AssetManager& asset_mgr, const base::xml::Element* properties) {
    return Style::createStyle(asset_mgr, properties);
}

void Style::deleteAsset(kernel::AssetManager&, std::unique_ptr<Style>) {
}

std::unique_ptr<Style> Style::createStyle(
    kernel::AssetManager& asset_mgr, const base::xml::Element* properties) {
    if(!properties) return nullptr;

    auto style = std::make_unique<Style>();

    kernel::Asset<kernel::TextureAtlas> texture_array;
    if(texture_array.request(asset_mgr, properties->FirstChildElement("TextureAtlas")))
        style->setTextureAtlas(std::move(texture_array));

    kernel::Asset<kernel::Font> font;
    if(font.request(asset_mgr, properties->FirstChildElement("Font")))
        style->setFont(std::move(font));

    for(auto widget : base::xml::IterateChildElements(properties, "WidgetPrototype")) {}

    return style;
}

} // namespace bembel::gui
