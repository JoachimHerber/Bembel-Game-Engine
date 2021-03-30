#ifndef BEMBEL_KERNEL_RENDERING_FONT_HPP
#define BEMBEL_KERNEL_RENDERING_FONT_HPP

#include <bembel/kernel/assets/asset-handle.hpp>
#include <bembel/kernel/assets/asset.hpp>
#include <bembel/kernel/assets/serial-asset-loader.hpp>
#include <bembel/library.hpp>

#include "texture.hpp"

namespace bembel::kernel {

class BEMBEL_API Font final {
  public:
    using DefaultLoaderType = SerialAssetLoader<Font>;

  public:
    Font();
    Font(const Font&)            = delete;
    Font& operator&(const Font&) = delete;
    ~Font();

    float getAscender() const;
    float getDescender() const;

    Texture* getGlyphAtlasTexture() const;

    static constexpr unsigned INVALIDE_GLYPH_INDEX = ~0u;
    unsigned                  getGlyphIndex(char32_t character, bool bold, bool oblique) const;

    float getAdvance(std::vector<unsigned>& glyph_indices) const;
    float getAdvance(unsigned glyph_undex) const;
    float getKernig(unsigned left_glyph_index, unsigned right_glyph_index) const;

    struct Glyph {
        float     advance;
        glm::vec2 extents_min;
        glm::vec2 extents_max;
        glm::vec2 tex_coords_min;
        glm::vec2 tex_coords_max;
    };
    const Glyph& getGlypData(unsigned glyph_index) const;

    unsigned getNumGlyphs() const;

    const static std::string& getTypeName();

    static std::unique_ptr<Font> loadAsset(AssetManager&, const std::string& path);
    static std::unique_ptr<Font> createAsset(AssetManager&, const xml::Element* properties);
    static void                  deleteAsset(AssetManager&, std::unique_ptr<Font>);

  private:
    bool readGlyphs(const xml::Element* properties);
    bool readCharMap(const xml::Element* properties);
    bool readKerning(const xml::Element* properties);

  private:
    using CharMap   = std::map<char32_t, unsigned>;
    using KernigMap = std::map<std::pair<unsigned, unsigned>, float>;

    enum CharMapIndex { Default, Oblique, Bold, BoldOblique };

    Asset<Texture> glyph_atlas_texture;

    float ascender  = 0.0f;
    float descender = 0.0f;

    std::vector<Glyph> glypths;

    std::array<CharMap, 4> char_maps;
    KernigMap              kernig;
};

} // namespace bembel::kernel

#endif // include guards
