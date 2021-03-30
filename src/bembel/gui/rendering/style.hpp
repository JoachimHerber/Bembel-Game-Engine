#ifndef BEMBEL_GUI_RENDERING_SKIN_HPP
#define BEMBEL_GUI_RENDERING_SKIN_HPP

#include <bembel/kernel/assets/asset.hpp>
#include <bembel/kernel/assets/serial-asset-loader.hpp>
#include <bembel/kernel/rendering/font.hpp>
#include <bembel/kernel/rendering/texture-atlas.hpp>
#include <bembel/kernel/rendering/texture.hpp>

namespace bembel::gui {
class WidgetView;

class BEMBEL_API Style {
  public:
    using DefaultLoaderType = kernel::SerialAssetLoader<Style>;

  public:
    Style();
    ~Style();

    void          setFont(kernel::Asset<kernel::Font> font);
    kernel::Font* getFont();

    void setTextureAtlas(kernel::Asset<kernel::TextureAtlas>);

    enum class Colors : unsigned {
        TEXT = 0,
        TEXT_OUTLINE,
        TEXT_DISABLED,
        WINDOW_BACKGROUND,
        WINDOW_BORDER,
        MENU_BAR_BACKGROUND,
        BUTTON_BACKGROUND,
        BUTTON_BACKGROUND_DISABLED,
        BUTTON_BACKGROUND_HOVERED,
        BUTTON_BACKGROUND_ACTIVE,
        BUTTON_BORDER,
        BUTTON_BORDER_DISABLED,
        BUTTON_BORDER_HOVERED,
        BUTTON_BORDER_ACTIVE,
        COUNT // for internal use only
    };
    const glm::tvec4<uint8_t>& getColor(Colors color);

    enum class Values : unsigned {
        MIN_FONT_SIZE = 0,
        WINDOW_BORDER_WIDTH,
        WINDOW_TITLE_BAR_HEIGHT,
        BUTTON_BORDER_WIDTH,
        COUNT // for internal use only
    };
    float getValue(Values value);

    kernel::Texture* getMainTexture() const;
    kernel::Texture* getFontTexture() const;

    const std::pair<glm::vec2, glm::vec2>* getTextureCoords(const std::string& name) const;

    static const std::string& getTypeName();

    static std::unique_ptr<Style> loadAsset(
        kernel::AssetManager& asset_mgr, const std::string& file_name);

    static std::unique_ptr<Style> createAsset(
        kernel::AssetManager& asset_mgr, const base::xml::Element* properties);
    static void deleteAsset(kernel::AssetManager& asset_mgr, std::unique_ptr<Style> _skin);

  private:
    static std::unique_ptr<Style> createStyle(
        kernel::AssetManager& asset_mgr, const base::xml::Element* properties);

  private:
    kernel::Asset<kernel::TextureAtlas> texture_atlas;

    kernel::Asset<kernel::Font> font;

    std::vector<glm::tvec4<uint8_t>> colors;
    std::vector<float>               values;
};

} // namespace bembel::gui
#endif // include guards
