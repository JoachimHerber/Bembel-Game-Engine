module;
#include <string_view>
#include <span>
#include <memory>
#include <filesystem>
#include <map>
export module bembel.text:Font;

import bembel.base;
import bembel.kernel;

namespace bembel::text {
using namespace bembel::base;
using namespace bembel::kernel;

export using GlyphIndex = uint;

export class Font final {
  public:
    static constexpr GlyphIndex INVALIDE_GLYPH_INDEX = ~0u;

  public:
    Font()                       = default;
    Font(Font const&)            = delete;
    Font& operator&(Font const&) = delete;
    ~Font()                      = default;

    float getAscender() const { return m_ascender; }
    float getDescender() const { return m_descender; }

    Texture* getGlyphAtlasTexture() const { return m_glyph_atlas_texture.get(); }

    GlyphIndex getGlyphIndex(char32_t character, bool bold, bool oblique) const;

    float getAdvance(std::span<GlyphIndex> glyph_indices) const;
    float getAdvance(GlyphIndex glyph_index) const;
    float getKernig(GlyphIndex left, GlyphIndex right) const;

    struct Glyph {
        float advance;
        vec2  extents_min;
        vec2  extents_max;
        vec2  tex_coords_min;
        vec2  tex_coords_max;
    };
    Glyph const& getGlypData(GlyphIndex glyph_index) const;

    uint getNumGlyphs() const { return m_glypths.size(); }

    static constexpr std::string_view ASSET_TYPE_NAME = "Font";

    static std::unique_ptr<Font> loadAsset(std::filesystem::path path);
    static std::unique_ptr<Font> createAsset(xml::Element const* properties);

    using DefaultLoaderType = SerialAssetLoader<Font>;

  private:
    bool readGlyphs(xml::Element const* properties);
    bool readCharMap(xml::Element const* properties);
    bool readKerning(xml::Element const* properties);

  private:
    using CharMap   = std::map<char32_t, GlyphIndex>;
    using KernigMap = std::map<std::pair<GlyphIndex, GlyphIndex>, float>;

    enum CharMapIndex { Default, Oblique, Bold, BoldOblique };

    Asset<Texture> m_glyph_atlas_texture;

    float m_ascender  = 0.0f;
    float m_descender = 0.0f;

    std::vector<Glyph> m_glypths;

    CharMap   m_char_maps[4];
    KernigMap m_kernig;
};

} // namespace bembel::text
