module;
#include "bembel/pch.h"
export module bembel.gui.core:Style;

import bembel.base;
import bembel.kernel;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class Style {
  public:
    Style()  = default;
    ~Style() = default;

    void          setFont(Asset<Font> font) { m_font = font; }
    kernel::Font* getFont() const { return m_font.getAsset(); }

    void setTextureAtlas(Asset<TextureAtlas>);

    enum class Colors : u32 {
        TEXT = 0,
        TEXT_OUTLINE,
        TEXT_DISABLED,
        TEXT_BACKGROUND,

        WINDOW_BACKGROUND,
        WINDOW_BORDER,
        MENU_BAR_BACKGROUND,

        BUTTON_BACKGROUND,
        BUTTON_BACKGROUND_HOVERED,
        BUTTON_BACKGROUND_SELECTED,
        BUTTON_BACKGROUND_SELECTED_AND_HOVERED,
        BUTTON_BACKGROUND_DISABLED,
        BUTTON_BACKGROUND_ACTIVE,

        BUTTON_BORDER,
        BUTTON_BORDER_HOVERED,
        BUTTON_BORDER_SELECTED,
        BUTTON_BORDER_SELECTED_AND_HOVERED,
        BUTTON_BORDER_DISABLED,
        BUTTON_BORDER_ACTIVE,

        INPUT_BORDER,
        INPUT_BACKGROUND,
        INPUT_BACKGROUND_DISABLED,
        INPUT_BACKGROUND_HOVERED,
        INPUT_BACKGROUND_SELECTED,

        CHECKBOX_BORDER,
        CHECKBOX_BACKGROUND,
        CHECKBOX_BACKGROUND_DISABLED,
        CHECKBOX_SELECTED,

        COUNT // for internal use only
    };
    glm::tvec4<uint8_t> const& getColor(Colors color) const;

    enum class Values : u32 {
        MIN_FONT_SIZE = 0,
        WINDOW_BORDER_WIDTH,
        WINDOW_TITLE_BAR_HEIGHT,
        BUTTON_BORDER_WIDTH,
        INPUT_BORDER_WIDTH,
        INPUT_VERTICAL_MARGIN,
        INPUT_HORIZONTAL_MARGIN,
        INPUT_CURSOR_BLINK_DURATION,
        INPUT_CURSOR_BLINK_ON_DURATION,
        CHECKBOX_SIZE,
        CHECKBOX_LABLE_MARGIN,
        COUNT // for internal use only
    };

    float getValue(Values value) const;

    Texture* getMainTexture() const;
    Texture* getFontTexture() const;

    TextureAtlas::Entry const* getTextureCoords(std::string_view name) const;

    static constexpr std::string_view ASSET_TYPE_NAME = "GuiStyle";

    static std::unique_ptr<Style> loadAsset(AssetManager& asset_mgr, std::filesystem::path file);

    static std::unique_ptr<Style> createAsset(
        AssetManager& asset_mgr, xml::Element const* properties
    );
    static void deleteAsset(AssetManager& asset_mgr, std::unique_ptr<Style> _skin);

    using DefaultLoaderType = kernel::SerialAssetLoader<Style>;

  private:
    static std::unique_ptr<Style> createStyle(
        kernel::AssetManager& asset_mgr, base::xml::Element const* properties
    );

  private:
    Asset<TextureAtlas> m_texture_atlas;

    Asset<Font> m_font;

    ColorRGBA m_colors[usize(Colors::COUNT)]{
        {0xff, 0xff, 0xff, 0xff}, // TEXT
        {0x00, 0x00, 0x00, 0xff}, // TEXT_OUTLINE
        {0x80, 0x80, 0x80, 0xff}, // TEXT_DISABLED
        {0x00, 0x00, 0x00, 0xff}, // TEXT_BACKGROUND

        {0x0f, 0x0f, 0x0f, 0xf0}, // WINDOW_BACKGROUND
        {0x0c, 0x0c, 0x0c, 0xff}, // WINDOW_BORDER
        {0x24, 0x24, 0x24, 0xff}, // MENU_BAR_BACKGROUND

        {0x42, 0x96, 0xfa, 0x66}, // BUTTON_BACKGROUND
        {0x42, 0x96, 0xfa, 0xff}, // BUTTON_BACKGROUND_HOVERED
        {0x42, 0x96, 0xfa, 0xaa}, // BUTTON_BACKGROUND_SELECTED
        {0x42, 0x96, 0xfa, 0xff}, // BUTTON_BACKGROUND_SELECTED_AND_HOVERED
        {0x80, 0x80, 0x80, 0x66}, // BUTTON_BACKGROUND_DISABLED
        {0x0f, 0x87, 0xfa, 0xff}, // BUTTON_BACKGROUND_ACTIVE

        {0x35, 0x78, 0xc8, 0x66}, // BUTTON_BORDER
        {0x35, 0x78, 0xc8, 0xff}, // BUTTON_BORDER_HOVERED
        {0x35, 0x78, 0xc8, 0xaa}, // BUTTON_BORDER_SELECTED
        {0x35, 0x78, 0xc8, 0xff}, // BUTTON_BORDER_SELECTED_AND_HOVERED
        {0x66, 0x66, 0x66, 0x66}, // BUTTON_BORDER_DISABLED
        {0x0c, 0x6c, 0xc8, 0xff}, // BUTTON_BORDER_ACTIVE

        {0x04, 0x04, 0x07, 0xff}, // INPUT_BORDER
        {0x42, 0x96, 0xfa, 0x66}, // INPUT_BACKGROUND
        {0x80, 0x80, 0x80, 0x66}, // INPUT_BACKGROUND_DISABLED
        {0x42, 0x96, 0xfa, 0xff}, // INPUT_BACKGROUND_HOVERED
        {0x0f, 0x87, 0xfa, 0xff}, // INPUT_BACKGROUND_SELECTED

        {0x04, 0x04, 0x07, 0xff}, // CHECKBOX_BORDER
        {0x42, 0x96, 0xfa, 0x33}, // CHECKBOX_BACKGROUND
        {0x04, 0x04, 0x07, 0x66}, // CHECKBOX_BACKGROUND_DISABLED
        {0x04, 0x04, 0x07, 0xff}  // CHECKBOX_SELECTED
    };

    float m_values[usize(Values::COUNT)]{
        28,   // MIN_FONT_SIZE
        4,   // WINDOW_BORDER_WIDTH
        32,  // WINDOW_TITLE_BAR_HEIGHT
        4,   // BUTTON_BORDER_WIDTH
        2,   // INPUT_BORDER_WIDTH
        2,   // INPUT_VERTICAL_MARGIN
        8,   // INPUT_HORIZONTAL_MARGIN
        1.0, // INPUT_CURSOR_BLINK_DURATION
        0.6, // INPUT_CURSOR_BLINK_ON_DURATION
        16,  // CHECKBOX_SIZE
        4,   // CHECKBOX_LABLE_MARGIN
    };
};

} // namespace bembel::gui
