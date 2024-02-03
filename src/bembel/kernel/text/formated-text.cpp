module;
#include <limits>
#include <string_view>
module bembel.kernel.text;

import bembel.base;

namespace bembel::kernel {
using namespace bembel::base;

bool Text::parse(In<std::u8string_view> text) {
    if(!m_font) return false;

    m_text.clear();
    m_text.emplace_back();
    m_glyphs.clear();
    m_glyphs.reserve(text.size());

    bool bold    = false;
    bool oblique = false;

    usize wordBegin = std::numeric_limits<u64>::max();

    std::string format;

    for(utf8::Iterator it = text; it; ++it) {
        char32_t c = *it;
        if(c != '[' && c != ' ' && c != '\t' && c != '\n') {
            GlyphIndex i = m_font->getGlyphIndex(c, bold, oblique);
            if(i != SdfFont::INVALIDE_GLYPH_INDEX) {
                m_glyphs.emplace_back(i);
                if(wordBegin > m_glyphs.size() - 1) { wordBegin = m_glyphs.size() - 1; }
            }
            continue;
        }

        if(wordBegin < m_glyphs.size()) {
            std::span<GlyphIndex> word{&m_glyphs[wordBegin], m_glyphs.size() - wordBegin};
            m_text.emplace_back(word);
            wordBegin = std::numeric_limits<u64>::max();
        }

        if(c == '\t') {
            m_text.emplace_back(Formating(Formating::TAB, 0));
        } else if(c == '\n') {
            m_text.emplace_back(Formating(Formating::NEW_LINE, 0));
        } else if(c == '[') {
            format.clear();
            for(++it; it; ++it) {
                char32_t c = *it;
                if(c == ']') break;
                if(c > 0b0111'1111) return false;

                format.push_back(char8_t(c));
            }
            if(!applyFormating(format, bold, oblique)) return false;
        }
    }
    return true;
}

bool Text::applyFormating(std::string_view format, bool& bold, bool& oblique) {
    // clang-format off
    if(format == "b")    { bold = true;                                                             return true; }
    if(format == "/b")   { bold = false;                                                            return true; }
    if(format == "i")    { oblique = true;                                                          return true; }
    if(format == "/i")   { oblique = false;                                                         return true; }
    if(format == "/a:l") { m_text.emplace_back(Formating(Formating::ALIGNMENT, Alignment::RIGHT));  return true; }
    if(format == "/a:r") { m_text.emplace_back(Formating(Formating::ALIGNMENT, Alignment::LEFT));   return true; }
    if(format == "/a:c") { m_text.emplace_back(Formating(Formating::ALIGNMENT, Alignment::CENTER)); return true; }
    if(format == "/a:b") { m_text.emplace_back(Formating(Formating::ALIGNMENT, Alignment::BLOCK));  return true; }
    // clang-format on
    return false;
}

} // namespace bembel::kernel
