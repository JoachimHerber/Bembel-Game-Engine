#ifndef BEMBEL_TOOLS_FONTCONVERTER_FONT_FAMILY_H
#define BEMBEL_TOOLS_FONTCONVERTER_FONT_FAMILY_H

#include <bembel/bembel.hpp>

// FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_IMAGE_H

#include "./glyph-texture-atlas.hpp"
#include "./glyph.hpp"

class FontFamily {
  public:
    FontFamily(const std::string& name, unsigned int units_per_EM);
    ~FontFamily() = default;

    bool addFace(const FT_Face& face);

    bool parseGlypes(const std::vector<char32_t>& characters);

    size_t getGlypheID(char32_t, bool, bool);

    std::vector<Glyph>&       getGlyphs();
    const std::vector<Glyph>& getGlyphs() const;

    const std::string& getName();
    unsigned           getUnitsPerEM();

    GlyphTextureAtlas& getTextureAtlas();

  private:
    using CharMap    = std::map<char32_t, unsigned int>;
    using KerningMap = std::map<std::pair<char32_t, char32_t>, float>;

    struct Face {
        Face(FT_Face face)
        : face{face} {}

        FT_Face face;
        CharMap charMap;
    };

  private:
    std::string name;

    unsigned int units_per_EM = 1;

    std::array<std::unique_ptr<Face>, 4> faces;

    std::vector<Glyph> glyphs;
    KerningMap         kerning;

    GlyphTextureAtlas texture_atlas;
};

#endif // include guards
