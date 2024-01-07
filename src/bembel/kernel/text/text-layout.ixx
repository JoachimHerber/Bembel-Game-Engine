module;
export module bembel.kernel.text:TextLayout;

import bembel.base;
import bembel.kernel.assets;
import :SdfFont;
import :Text;

namespace bembel::kernel {
using namespace bembel::base;

export class TextLayout final {
  public:
    TextLayout()  = default;
    ~TextLayout() = default;

    struct Glyph {
        GlyphIndex glyph;
        vec2       pos;

        Glyph(GlyphIndex g, float x, float y) : glyph{g}, pos{x, y} {}
        Glyph(Glyph&&)            = default;
        Glyph& operator=(Glyph&&) = default;
    };

    uint calculateMinWidth(In<Text> text, In<float> font_size, In<std::optional<uint>> height)
        const;
    uint calculateMinHeight(In<Text> text, In<float> font_size, In<std::optional<uint>> width)
        const;

    bool calculateSimpleLayout(
        Text const& text, vec2 origin, float font_size, float max_line_length
    );

    std::vector<Glyph> const& getGlyphs() const { return m_glyphs; }

  private:
    std::vector<Glyph> m_glyphs;
    vec2               m_origin;
};

} // namespace bembel::kernel
