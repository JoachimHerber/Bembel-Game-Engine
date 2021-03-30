#include "text-layout.hpp"

namespace bembel::kernel {

UTF8TextIterator::UTF8TextIterator(std::string_view text, std::size_t index)
: text{text}
, index{index} {
}

UTF8TextIterator UTF8TextIterator::begin() {
    return UTF8TextIterator(this->text);
}

UTF8TextIterator UTF8TextIterator::end() {
    return UTF8TextIterator(this->text, this->text.length());
}

char32_t UTF8TextIterator::operator*() {
    unsigned numBytes = this->getNumBytes();
    if(numBytes == 0 || this->index + numBytes > this->text.length()) return 0;

    char32_t character = this->text[this->index];
    switch(numBytes) {
        case 1: character &= 0b0111'1111; break;
        case 2: character &= 0b0001'1111; break;
        case 3: character &= 0b0000'1111; break;
        case 4: character &= 0b0000'0111; break;
        case 5: character &= 0b0000'0011; break;
        case 6: character &= 0b0000'0001; break;
    }
    for(int i = 1; i < numBytes; ++i) {
        char c = this->text[this->index + i];
        if((c & 0b1100'0000) != 0b1000'000) return 0; // invalid byte

        character = character << 6 | (c & 0b0011'1111);
    }
    return character;
}

bool UTF8TextIterator::operator!=(const UTF8TextIterator& other) {
    return this->index != other.index;
}

UTF8TextIterator& UTF8TextIterator::operator++() {
    unsigned numBytes = this->getNumBytes();
    if(numBytes != 0) {
        this->index += numBytes;
    } else {
        this->index = this->text.length();
    }

    return *this;
}

unsigned UTF8TextIterator::getNumBytes() const {
    if(this->index >= this->text.size()) return 0;

    char c = this->text[this->index];

    if((c & 0b1000'0000) == 0) return 1; // one byte character
    if((c & 0b0100'0000) == 0) return 0; // follow byte of multi byte character
    if((c & 0b0010'0000) == 0) return 2; // two byte character
    if((c & 0b0001'0000) == 0) return 3; // three byte character
    if((c & 0b0000'1000) == 0) return 4; // four byte character
    if((c & 0b0000'0100) == 0) return 5; // five byte character
    if((c & 0b0000'0010) == 0) return 6; // six byte character
}

void TextLayout::setFont(Font* font) {
    this->font = font;
}

const Font* TextLayout::getFont() const {
    return this->font;
}

void TextLayout::setText(std::string_view text) {
    this->text = text;
}

void TextLayout::calculateSimpleLayout(
    VerticalAlignment v_align, HorizontalAlignment h_align, bool bold, bool oblique) {
    if(!this->font) return;

    this->glyphs.clear();
    std::vector<Line> lines;
    lines.push_back({0, 0, 0.0f});
    Line* cur_line = &(lines.back());

    unsigned prev_glyph_index = Font::INVALIDE_GLYPH_INDEX;
    for(char32_t c : UTF8TextIterator(this->text)) {
        if(c == '\n') {
            lines.push_back({this->glyphs.size(), 0, 0.0f});
            prev_glyph_index = Font::INVALIDE_GLYPH_INDEX;
            cur_line         = &(lines.back());
            continue;
        }
        auto glyph_index = font->getGlyphIndex(c, bold, oblique);
        if(glyph_index != Font::INVALIDE_GLYPH_INDEX) {
            cur_line->num_glyphs++;
            if(prev_glyph_index != Font::INVALIDE_GLYPH_INDEX) {
                cur_line->lenght += font->getKernig(prev_glyph_index, glyph_index);
            }

            this->glyphs.push_back({{cur_line->lenght, 0}, glyph_index});
            cur_line->lenght += font->getAdvance(glyph_index);
            prev_glyph_index = glyph_index;
        }
    }

    float line_height = font->getAscender() - font->getDescender();

    float y = 0;
    if(v_align != VerticalAlignment::Baseline) y -= font->getAscender();
    if(v_align == VerticalAlignment::Center) y += 0.5f * line_height * lines.size();
    if(v_align == VerticalAlignment::Buttom) y += line_height * lines.size();
    for(auto& line : lines) {
        float x_offset = 0;
        switch(h_align) {
            case HorizontalAlignment::Center: x_offset -= 0.5f * line.lenght; break;
            case HorizontalAlignment::Right: x_offset -= line.lenght; break;
        }
        for(size_t n = 0; n < line.num_glyphs; ++n) {
            auto& glyph = this->glyphs[line.first_glyph_index + n];
            glyph.pos.x += x_offset;
            glyph.pos.y = y;
        }
        y += line_height;
    }
}

} // namespace bembel::kernel
