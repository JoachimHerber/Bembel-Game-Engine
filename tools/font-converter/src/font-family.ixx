module;
#include <array>
#include <map>
#include <string_view>
#include <utility>
#include <memory>
//
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_IMAGE_H
export module bembel.tools.font_converter:FontFamily;

import bembel;
import :GlyphTextureAtlas;
import :Glyph;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::gui;

export class FontFamily {
  public:
    FontFamily(std::string_view name, unsigned int units_per_EM);
    ~FontFamily() = default;

    bool addFace(FT_Face const& face);

    enum FaceType { DEFAULT = 0, OBLIQUE, BOLD, BOLD_OBLIQUE };

    bool hasFace(FaceType type) { return m_faces[std::to_underlying(type)] != nullptr; }

    bool parseGlypes(std::span<char32_t> characters, std::span<FaceType> faces);

    size_t getGlypheID(char32_t, bool, bool);

    std::vector<Glyph>&       getGlyphs() { return m_glyphs; }
    std::vector<Glyph> const& getGlyphs() const { return m_glyphs; }

    const std::string_view getName() { return m_name; }
    unsigned               getUnitsPerEM() { return m_units_per_EM; }

    auto& getKerning() { return m_kerning; }

  private:
    using CharMap    = std::map<char32_t, usize>;
    using KerningMap = std::map<std::pair<char32_t, char32_t>, float>;

    struct Face {
        Face(FT_Face face) : face{face} {}

        FT_Face face;
        CharMap charMap;
    };

  private:
    std::string m_name;

    unsigned int m_units_per_EM = 1;

    std::array<std::unique_ptr<Face>, 4> m_faces;

    std::vector<Glyph> m_glyphs;
    KerningMap         m_kerning;
};
} // namespace bembel::tools
