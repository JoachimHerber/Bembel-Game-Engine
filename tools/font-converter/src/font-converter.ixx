module;
#include <string_view>
//
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_IMAGE_H
export module bembel.tools.font_converter:FontConverter;

import :FontFamily;
import :FontTextureGenerator;

namespace bembel::tools {
using bembel::base::Dictionary;
using bembel::base::In;
using bembel::base::Signal;
using bembel::base::uvec2;

export class FontConverter {
  public:
    FontConverter();
    ~FontConverter();

    GlyphTextureAtlas& getGlyphTextureAtlas() { return m_texture_atlas; }
    FontTextureGenerator& getTextureGenerator() { return m_texture_generator; }

    bool loade(In<std::filesystem::path>);

    void setSelectedFontFamily(int selection) {
        if(0 <= selection && selection < m_fonts.size()) {
            m_selected_font_family = m_fonts[selection].get();
        } else {
            m_selected_font_family = nullptr;
        }
    }

    FontFamily* getFontFamily() { return m_selected_font_family; }
    void        setResolution(uvec2 res);

    bool save(In<std::filesystem::path>);

    void converSelectedFont(
        std::vector<char32_t> characters, std::span<FontFamily::FaceType> faces, double max_dist
    );

    Signal<std::u8string_view> font_family_added_signal;

  private:
    bool loadeFile(In<std::filesystem::path>);

    void saveGlypes(bembel::base::xml::Element* root);
    void saveCharMap(bembel::base::xml::Element* root);
    void saveKerning(bembel::base::xml::Element* root);

  private:
    FT_Library              m_library;
    Dictionary<FontFamily*> m_font_dictionary;

    FontFamily* m_selected_font_family = nullptr;

    std::vector<std::unique_ptr<FontFamily>> m_fonts;

    GlyphTextureAtlas    m_texture_atlas;
    FontTextureGenerator m_texture_generator;

    std::vector<char32_t> m_characters;
    std::array<bool, 4>   m_faces;
};

} // namespace bembel::tools
