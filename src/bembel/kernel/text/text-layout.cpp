module;
#include <span>
#include <string>
#include <variant>
module bembel.kernel.text;

namespace bembel::kernel {
using namespace bembel::base;

uint TextLayout::calculateMinWidth(
    In<Text> text, In<float> font_size, In<std::optional<uint>> height
) const {
    SdfFont const* const font = text.getFont();
    if(!font) return 0;

    uint max_word_length = 0;
    for(auto& text_element : text.getText()) {
        if(std::holds_alternative<std::span<GlyphIndex>>(text_element)) {
            auto glyphs = std::get<std::span<GlyphIndex>>(text_element);
            if(glyphs.empty()) continue;

            uint word_length = 0;
            for(GlyphIndex prev = 0; GlyphIndex glyphIndex : glyphs) {
                if(word_length != 0) word_length += font_size * font->getKernig(prev, glyphIndex);

                word_length += font_size * font->getAdvance(glyphIndex);
                prev = glyphIndex;
            }
            max_word_length = std::max(max_word_length, word_length);
        }
    }
    return max_word_length;
}
uint TextLayout::calculateMinHeight(
    In<Text> text, In<float> font_size, In<std::optional<uint>> width
) const {
    SdfFont const* const font = text.getFont();
    if(!font) return 0;

    float const space = font_size * font->getAdvance(font->getGlyphIndex(' ', false, false));

    uint min_height  = 2 * font_size;
    uint line_length = 0;
    for(auto& text_element : text.getText()) {
        if(std::holds_alternative<Text::Formating>(text_element)) {
            auto formating = std::get<Text::Formating>(text_element);
            if(formating.option == Text::Formating::NEW_LINE) {
                min_height += font_size;
                line_length = 0;
            }
        } else {
            auto glyphs = std::get<std::span<GlyphIndex>>(text_element);
            if(glyphs.empty()) continue;

            uint word_length = 0;
            for(GlyphIndex prev = 0; GlyphIndex glyphIndex : glyphs) {
                if(word_length != 0) word_length += font_size * font->getKernig(prev, glyphIndex);

                word_length += font_size * font->getAdvance(glyphIndex);
                prev = glyphIndex;
            }
            if(width && line_length + space + word_length > *width) {
                min_height += font_size;
                line_length = word_length;
            } else {
                line_length += (line_length == 0 ? space : 0) + word_length;
            }
        }
    }
    return min_height;
}

bool TextLayout::calculateSimpleLayout(
    Text const& text, vec2 origin, float font_size, float max_line_length
) {
    SdfFont const* const font = text.getFont();

    if(!font) return false;

    float const space = font_size * font->getAdvance(font->getGlyphIndex(' ', false, false));

    m_glyphs.clear();
    m_glyphs.reserve(text.getNumGlyphs());

    struct Word {
        std::span<Glyph> glyphs;
        float            advance;
    };
    std::vector<Word> words;
    float             y           = origin.y;
    float             line_length = 0;
    Text::Alignment   align       = Text::Alignment::LEFT;

    auto EndLine = [&]() {
        if(words.empty()) {
            y -= font_size;
            return;
        }

        float word_spacing = space;
        float x            = origin.x;
        switch(align) {
            case Text::Alignment::RIGHT: x = max_line_length - line_length; break;
            case Text::Alignment::CENTER: x = (max_line_length - line_length) / 2; break;
            case Text::Alignment::BLOCK:
                if(words.size() > 1)
                    word_spacing += (max_line_length - line_length) / (words.size() - 1);
                break;
        }
        for(auto& word : words) {
            for(auto& glyph : word.glyphs) {
                glyph.pos.x += x;
                glyph.pos.y = y;
            }
            x += word.advance + word_spacing;
        }
        words.clear();
        y -= font_size;
        line_length = 0;
    };

    for(auto& text_element : text.getText()) {
        if(std::holds_alternative<Text::Formating>(text_element)) {
            auto formating = std::get<Text::Formating>(text_element);
            if(formating.option == Text::Formating::ALIGNMENT) {
                align = Text::Alignment(formating.value);
            } else if(formating.option == Text::Formating::NEW_LINE) {
                EndLine();
            }
        } else {
            auto glyphs = std::get<std::span<GlyphIndex>>(text_element);
            if(glyphs.empty()) continue;

            Word  w{{}, 0};
            usize word_begin = m_glyphs.size();

            for(GlyphIndex prev = 0; GlyphIndex glyphIndex : glyphs) {
                if(w.advance != 0) w.advance += font_size * font->getKernig(prev, glyphIndex);

                m_glyphs.emplace_back(glyphIndex, w.advance, 0.0f);
                w.advance += font_size * font->getAdvance(glyphIndex);
                prev = glyphIndex;
            }
            w.glyphs = std::span<Glyph>{&m_glyphs[word_begin], m_glyphs.size() - word_begin};

            if(line_length == 0) {
                line_length = w.advance;
                words.push_back(w);
            } else if(line_length + space + w.advance < max_line_length) {
                line_length += w.advance + space;
                words.push_back(w);
            } else {
                EndLine();
                words.push_back(w);
                line_length = w.advance;
            }
        }
    }
    if(!words.empty()) EndLine();

    return true;
}

} // namespace bembel::kernel
