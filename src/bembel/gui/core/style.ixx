module;
#include <string_view>
export module bembel.gui.core:Style;

import bembel.base;
import bembel.kernel;
import bembel.text;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::text;

export class Style {
  public:
    Style()  = default;
    ~Style() = default;

    void  setFont(Asset<Font> font) { m_font = std::move(font); }
    Font* getFont() const { return m_font.get(); }

    void setTextureAtlas(Asset<TextureAtlas>);

    enum class Colors : u32 {
        TEXT = 0,
        TEXT_DISABLED,
        TEXT_OUTLINE,

        WINDOW_BACKGROUND,
        WINDOW_BORDER,
        MENU_BAR_BACKGROUND,

        BORDER,
        BORDER_DISABLED,
        BORDER_HOVERED,
        BORDER_ACTIVE,

        BUTTON,
        BUTTON_DISABLED,
        BUTTON_HOVERED,
        BUTTON_ACTIVE,

        INPUT,
        INPUT_DISABLED,
        INPUT_HOVERED,
        INPUT_ACTIVE,

        CHECKBOX,
        CHECKBOX_DISABLED,
        CHECKBOX_HOVERED,

        SLIDER,
        SLIDER_DISABLED,
        SLIDER_HOVERED,
        SLIDER_ACTIVE,

        COUNT // for internal use only
    };
    ColorRGBA const& getColor(Colors color) const;

    enum class Values : u32 {
        MIN_FONT_SIZE = 0,
        WINDOW_BORDER_WIDTH,
        WINDOW_TITLE_BAR_HEIGHT,
        BUTTON_CORNER_SIZE,
        BUTTON_TEXT_MARGIN,
        INPUT_BORDER_WIDTH,
        INPUT_VERTICAL_MARGIN,
        INPUT_HORIZONTAL_MARGIN,
        INPUT_CURSOR_BLINK_DURATION,
        INPUT_CURSOR_BLINK_ON_DURATION,
        CHECKBOX_SIZE,
        CHECKBOX_LABLE_MARGIN,
        SLIDER_HANDLE_WIDTH,
        TAB_BAR_HEIGHT,
        TAB_MARGIN,
        COUNT // for internal use only
    };

    float getValue(Values value) const;

    Texture* getMainTexture() const;
    Texture* getFontTexture() const;

    TextureAtlas::Entry const* getTextureCoords(std::string_view name) const;

    static constexpr std::string_view ASSET_TYPE_NAME = "GuiStyle";

    static std::unique_ptr<Style> loadAsset(std::filesystem::path file);
    static std::unique_ptr<Style> createAsset(xml::Element const* properties);

    using DefaultLoaderType = kernel::SerialAssetLoader<Style>;

  private:
    static std::unique_ptr<Style> createStyle(xml::Element const* properties);

  private:
    Asset<TextureAtlas> m_texture_atlas;

    Asset<Font> m_font;

    ColorRGBA u32ToColor(u32 c) { return {u8(c >> 24), u8(c >> 16), u8(c >> 8), u8(c)}; }

    ColorRGBA m_colors[usize(Colors::COUNT)]{
        u32ToColor(0xff'ff'ff'ff), // TEXT
        u32ToColor(0x80'80'80'ff), // TEXT_DISABLED
        u32ToColor(0x00'00'00'ff), // TEXT_OUTLINE

        u32ToColor(0x0f'0f'0f'f0), // WINDOW_BACKGROUND
        u32ToColor(0x0c'0c'0c'ff), // WINDOW_BORDER
        u32ToColor(0x24'24'24'ff), // MENU_BAR_BACKGROUND

        u32ToColor(0x0a'17'26'ff), // BORDER
        u32ToColor(0x0a'17'26'ff), // BORDER_DISABLED
        u32ToColor(0x11'26'40'ff), // BORDER_HOVERED
        u32ToColor(0x11'26'40'ff), // BORDER_ACTIVE

        u32ToColor(0x22'4d'80'ff), // BUTTON
        u32ToColor(0x2c'2c'2c'66), // BUTTON_DISABLED
        u32ToColor(0x19'48'80'ff), // BUTTON_HOVERED
        u32ToColor(0x11'3e'73'ff), // BUTTON_ACTIVE

        u32ToColor(0x04'04'07'ff), // INPUT
        u32ToColor(0x80'80'80'66), // INPUT_DISABLED
        u32ToColor(0x0a'0d'13'ff), // INPUT_HOVERED
        u32ToColor(0x08'0a'0f'ff), // INPUT_ACTIVE

        u32ToColor(0x14'3a'66'ff), // CHECKBOX
        u32ToColor(0x80'80'80'66), // CHECKBOX_DISABLED
        u32ToColor(0x1f'57'99'ff), // CHECKBOX_HOVERED

        u32ToColor(0x22'4d'80'ff), // SLIDER
        u32ToColor(0x2c'2c'2c'66), // SLIDER_DISABLED
        u32ToColor(0x33'79'cc'ff), // SLIDER_HOVERED
        u32ToColor(0x26'6c'bf'ff), // SLIDER_ACTIVE

    };

    float m_values[usize(Values::COUNT)]{
        28,  // MIN_FONT_SIZE
        4,   // WINDOW_BORDER_WIDTH
        32,  // WINDOW_TITLE_BAR_HEIGHT
        8,   // BUTTON_CORNER_SIZE
        4,   // BUTTON_TEXT_MARGIN
        2,   // INPUT_BORDER_WIDTH
        2,   // INPUT_VERTICAL_MARGIN
        8,   // INPUT_HORIZONTAL_MARGIN
        1.0, // INPUT_CURSOR_BLINK_DURATION
        0.6, // INPUT_CURSOR_BLINK_ON_DURATION
        16,  // CHECKBOX_SIZE
        4,   // CHECKBOX_LABLE_MARGIN
        16,  // SLIDER_HANDLE_WIDTH
        32,  // TAB_BAR_HEIGHT
        16   // TAB_MARGIN
    };
};

} // namespace bembel::gui
