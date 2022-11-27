module;
#include "bembel/pch.h"
module bembel.gui.widgets;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

LabelWidget::LabelWidget(Widget& parent, std::u8string_view text) : Widget{parent}, m_text{text} {
    m_view = std::make_unique<LabelWidget::View>(*this);
    updateGlyphs();
}

bool LabelWidget::configure(base::xml::Element const* properties) {
    Widget::configure(properties);
    if(!properties) return true;

    xml::getAttribute(properties, "text", m_text);
    xml::getAttribute(properties, "outline", m_outline);

    std::string alignment = "";
    if(xml::getAttribute(properties, "alignment", alignment)) {
        std::transform(alignment.begin(), alignment.end(), alignment.begin(), [](char c) {
            return std::tolower(c);
        });
        if(alignment == "left") m_alignment = Alignment::Left;
        if(alignment == "right") m_alignment = Alignment::Right;
    }

    m_view = std::make_unique<LabelWidget::View>(*this);
    return true;
}

uint LabelWidget::getMinWidth() const {
    auto style = this->getStyle();
    assert(style && "GUI::Style is undefined");
    auto font = style->getFont();
    assert(font && "Font is undefined");

    float line_heigth = font->getAscender() - font->getDescender();
    float scale       = style->getValue(Style::Values::MIN_FONT_SIZE) / line_heigth;

    return scale * m_text_length;
}

uint LabelWidget::getMinHeight() const {
    auto style = this->getStyle();
    assert(style && "GUI::Style is undefined");
    auto font = style->getFont();
    assert(font && "Font is undefined");

    auto min_font_size = style->getValue(Style::Values::MIN_FONT_SIZE);
    if(m_outline) {
        float line_heigth = font->getAscender() - font->getDescender();
        min_font_size *= 1.0 + 0.05 / line_heigth;
    }
    return uint(min_font_size);
}

void LabelWidget::updateGlyphs() {
    auto style = this->getStyle();
    assert(style && "GUI::Style is undefined");
    auto font = style->getFont();
    assert(font && "Font is undefined");

    m_glyphs.clear();
    m_text_length       = 0;
    unsigned prev_index = kernel::Font::INVALIDE_GLYPH_INDEX;
    for(char32_t c : m_text) {
        unsigned index = font->getGlyphIndex(c, false, false);
        if(index == kernel::Font::INVALIDE_GLYPH_INDEX) continue;

        if(prev_index != kernel::Font::INVALIDE_GLYPH_INDEX) {
            m_text_length += font->getKernig(prev_index, index);
        }
        if(c != ' ') m_glyphs.emplace_back(index, m_text_length);

        m_text_length += font->getAdvance(index);
        prev_index = index;
    }
}

void LabelWidget::View::draw(RenderBatchInterface& batch) {
    auto style = m_label.getStyle();
    assert(style && "GUI::Style is undefined");
    auto font = style->getFont();
    assert(font && "Font is undefined");

    float border = 3;

    glm::vec2 size = m_label.size.get();

    float outline_margin = m_label.m_outline ? 0.05 : 0;
    float text_length    = m_label.m_text_length + outline_margin;
    float line_heigth    = font->getAscender() - font->getDescender() + outline_margin;
    float scale          = std::min(size.x / text_length, size.y / line_heigth);

    glm::vec2 pos = glm::vec2(m_label.position.get());
    switch(m_label.m_alignment) {
        case LabelWidget::Alignment::Center:
            pos.x += 0.5f * (size.x - scale * m_label.m_text_length);
            break;
        case LabelWidget::Alignment::Right: pos.x += size.x - scale * m_label.m_text_length; break;
    }
    pos.y +=
        0.5f * size.y - scale * (0.5 * line_heigth + font->getDescender()); // center text verticaly

    if(m_label.m_outline) {
        batch.setColor(style->getColor(Style::Colors::TEXT_OUTLINE));
        for(auto const& it : m_label.m_glyphs) {
            auto const& glyph = font->getGlypData(it.index);

            glm::vec2 p = pos;
            p.x += scale * it.x;
            batch.drawGlyph(glyph, p, scale, 0.20f, 0.30f);
        }
    }

    if(m_label.m_text_color.has_value()) {
        batch.setColor(m_label.m_text_color.value());
    } else {
        batch.setColor(style->getColor(Style::Colors::TEXT));
    }

    for(auto const& it : m_label.m_glyphs) {
        auto const& glyph = font->getGlypData(it.index);

        glm::vec2 p = pos;
        p.x += scale * it.x;
        batch.drawGlyph(glyph, p, scale);
    }
}

} // namespace bembel::gui
