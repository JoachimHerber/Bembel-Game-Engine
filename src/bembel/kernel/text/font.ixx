module;
#include <filesystem>
#include <map>
#include <memory>
#include <span>
#include <string_view>
export module bembel.kernel.text:SdfFont;

import bembel.base;
import bembel.kernel.assets;
import bembel.kernel.rendering;

namespace bembel::kernel {
using namespace bembel::base;

export using GlyphIndex = uint;

export class SdfFont final {
  public:
    static constexpr GlyphIndex INVALIDE_GLYPH_INDEX = ~0u;

  public:
    SdfFont()                          = default;
    SdfFont(SdfFont const&)            = delete;
    SdfFont& operator&(SdfFont const&) = delete;
    ~SdfFont()                         = default;

    float getAscender() const { return m_ascender; }
    float getDescender() const { return m_descender; }

    Texture* getGlyphAtlasTexture() const { return m_glyph_atlas_texture.get(); }

    GlyphIndex getGlyphIndex(char32_t character, bool bold, bool oblique) const;

    float getAdvance(std::span<GlyphIndex> glyph_indices) const;
    float getAdvance(GlyphIndex glyph_index) const;
    float getKernig(GlyphIndex left, GlyphIndex right) const;

    struct SubGlyph {
        GlyphIndex glyph;
        vec2       pos;
    };

    struct Glyph {
        float                 advance        = 0;
        vec2                  extents_min    = {0.0, 0.0};
        vec2                  extents_max    = {0.0, 0.0};
        vec2                  tex_coords_min = {0.0, 0.0};
        vec2                  tex_coords_max = {0.0, 0.0};
        std::vector<SubGlyph> subglyphs;
    };
    Glyph const& getGlypData(GlyphIndex glyph_index) const;

    uint getNumGlyphs() const { return m_glypths.size(); }

    static constexpr std::string_view ASSET_TYPE_NAME = "Font";

    static std::unique_ptr<SdfFont> loadAsset(std::filesystem::path path);
    static std::unique_ptr<SdfFont> createAsset(xml::Element const* properties);

    using DefaultLoaderType = SerialAssetLoader<SdfFont>;

  private:
    bool readGlyphs(xml::Element const* properties, In<uint> resolution, In<uint> units_per_em);
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

} // namespace bembel::kernel
