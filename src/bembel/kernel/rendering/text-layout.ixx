module;
export module bembel.kernel.rendering:TextLayout;

import bembel.base;
import bembel.kernel.assets;
import :Font;
import :Text;

namespace bembel::kernel {
using namespace bembel::base;

export class TextLayout final {
  public:
    TextLayout()  = default;
    ~TextLayout() = default;

    struct Glyph {
        Font::Glyph const* glyph;
        vec2               pos;

        Glyph(Font::Glyph const* g, float x, float y) : glyph{g}, pos{x, y} {}
        Glyph(Glyph&&)            = default;
        Glyph& operator=(Glyph&&) = default;
    };

    bool calculateSimpleLayout(
        Text const& text, vec2 origin, float font_size, float max_line_length
    );

    std::vector<Glyph> const& getGlyphs() const { return m_glyphs; }

  private:
    std::vector<Glyph> m_glyphs;
    vec2               m_origin;
};

} // namespace bembel::kernel
