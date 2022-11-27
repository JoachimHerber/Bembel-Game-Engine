﻿module;
#include "bembel/pch.h"
export module bembel.kernel.rendering:Text;

import bembel.base;
import bembel.kernel.assets;
import :Font;

namespace bembel::kernel {
using namespace bembel::base;

export class Text final {
  public:
    Text()  = default;
    ~Text() = default;

    void        setFont(Font* font) { m_font = font; }
    Font const* getFont() const { return m_font; }

    bool parse(In<String> text);

    enum Alignment : u32 { LEFT, RIGHT, CENTER, BLOCK };

    struct Formating {
        enum Option {
            COLOR,
            RESET_COLOR,
            NEW_LINE,
            TAB,
            ALIGNMENT,
        };

        Option option;
        u32    value;
    };

    using TextElement = std::variant<std::span<GlyphIndex>, Formating>;

    std::vector<TextElement> const& getText() const { return m_text; }
    usize                           getNumGlyphs() const { return m_glyphs.size(); };

  private:
    bool applyFormating(std::string_view, bool& bold, bool& oblique);

  private:
    Font*                    m_font = nullptr;
    std::vector<GlyphIndex>  m_glyphs;
    std::vector<TextElement> m_text;
};

} // namespace bembel::kernel
