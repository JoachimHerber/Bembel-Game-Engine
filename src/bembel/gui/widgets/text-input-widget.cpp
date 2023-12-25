module;
#include <cassert>
#include <chrono>
#include <memory>
#include <string_view>
module bembel.gui.widgets;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

TextInputWidget::TextInputWidget(Widget& parent) : Widget{parent} {
    m_interaction_handles.push_back(&m_handle);

    m_view = std::make_unique<TextInputWidget::View>(*this);

    this->size.change_signal.bind(this, &TextInputWidget::onSizeChanged);
    this->text.change_signal.bind(this, &TextInputWidget::onTextChanged);
    m_handle.action_signal.bind(this, &TextInputWidget::onAction);
    m_handle.text_input_signal.bind(this, &TextInputWidget::onTextInput);

    m_handle.cursor = Asset<CursorIcon>("IBeam");
}

bool TextInputWidget::configure(xml::Element const* properties) {
    Widget::configure(properties);
    if(!properties) return true;

    auto content = properties->FirstChildElement("Content");
    if(content) {
        char8_t* text = (char8_t*)content->GetText();
        this->text.set(std::u8string(text));
    }

    return true;
}

uint TextInputWidget::getMinWidth(In<std::optional<uint>>) const {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float border   = style->getValue(Style::Values::INPUT_BORDER_WIDTH);
    float margin = style->getValue(Style::Values::INPUT_HORIZONTAL_MARGIN);
    return uint(2 * border + 2 * margin);
}

uint TextInputWidget::getMinHeight(In<std::optional<uint>>) const {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float border   = style->getValue(Style::Values::INPUT_BORDER_WIDTH);
    float margin = style->getValue(Style::Values::INPUT_VERTICAL_MARGIN);
    return uint(2 * border + 2 * margin);
}

void TextInputWidget::onSizeChanged(In<ivec2>, In<ivec2> new_size) {
    m_handle.size = new_size;
}

void TextInputWidget::onAction(InteractionHandle::Action action, ivec2 cursor_pos) {
    switch(action) {
        case InteractionHandle::Action::INTERACT: break;
        case InteractionHandle::Action::DELETE:
            if(m_cursor_pos < text.get().size()) {
                std::u8string tmp = this->text.get();
                utf8::eraseCodePoint(tmp, m_cursor_pos);
                this->text.set(tmp);
            }
            break;
        case InteractionHandle::Action::BACKSPACE:
            if(m_cursor_pos > 0) {
                std::u8string tmp = this->text.get();
                --m_cursor_pos;
                utf8::eraseCodePoint(tmp, m_cursor_pos);
                this->text.set(tmp);
            }
            break;
        case InteractionHandle::Action::NAVIGATE_RIGHT:
            if(m_cursor_pos < text.get().size()) ++m_cursor_pos;
            break;
        case InteractionHandle::Action::NAVIGATE_LEFT:
            if(m_cursor_pos > 0) --m_cursor_pos;
            break;
    }
}

void TextInputWidget::onTextInput(char32_t c) {
    std::u8string tmp = this->text.get();
    utf8::insertCodePoint(tmp, m_cursor_pos, c);
    ++m_cursor_pos;
    this->text.set(tmp);
}

void TextInputWidget::onTextChanged(In<std::u8string>, In<std::u8string> str) {
    if(m_view) ((View*)m_view.get())->updateGlyphs(str);
}

void TextInputWidget::copy() {}

void TextInputWidget::paste() {}

void TextInputWidget::View::updateGlyphs(In<std::u8string> str) {
    auto style = m_widget.getStyle();
    assert(style && "GUI::Style is undefined");
    auto font = style->getFont();
    assert(font && "Font is undefined");

    m_glyphs.clear();
    m_advance = 0;
    for(unsigned prev_index = SdfFont::INVALIDE_GLYPH_INDEX; char32_t c : str) {
        auto const glyph_index = font->getGlyphIndex(c, false, false);

        if(prev_index != SdfFont::INVALIDE_GLYPH_INDEX)
            m_advance += font->getKernig(prev_index, glyph_index);

        m_glyphs.emplace_back(glyph_index, m_advance);
        m_advance += font->getAdvance(glyph_index);

        prev_index = glyph_index;
    }
}

void TextInputWidget::View::draw(RenderBatchInterface& batch) {
    auto style = m_widget.getStyle();
    assert(style && "GUI::Style is undefined");
    auto font = style->getFont();
    assert(font && "Font is undefined");

    vec2 min = m_widget.position.get();
    vec2 max = min + vec2(m_widget.size.get());

    drawBackground(batch, *style, min, max);

    float border   = style->getValue(Style::Values::INPUT_BORDER_WIDTH);
    float v_margin = style->getValue(Style::Values::INPUT_VERTICAL_MARGIN) + border;
    float h_margin = style->getValue(Style::Values::INPUT_HORIZONTAL_MARGIN) + border;

    float ascender    = font->getAscender();
    float descender   = font->getDescender();
    float line_heigth = ascender - descender;
    float scale       = (m_widget.size.get().y - 2 * v_margin) / line_heigth;
    vec2  text_origin = {min.x + h_margin, 0.5 * (max.y + min.y - scale * (ascender + descender))};

    drawText(batch, *style, *font, text_origin, scale);
    if(m_widget.isSelected())
        drawCursor(batch, *style, *font, text_origin, scale, m_widget.m_cursor_pos);
}

void TextInputWidget::View::drawBackground(
    RenderBatchInterface& batch, Style const& style, vec2 outer_min, vec2 outer_max
) {
    float border = style.getValue(Style::Values::INPUT_BORDER_WIDTH);

    vec2 inner_min = outer_min + vec2(border);
    vec2 inner_max = outer_max - vec2(border);

    if(m_widget.isDisabled()) {
        batch.setColor(style.getColor(Style::Colors::BORDER_DISABLED));
    } else if(m_widget.isSelected()) {
        batch.setColor(style.getColor(Style::Colors::BORDER_ACTIVE));
    } else if(m_widget.isHovered()) {
        batch.setColor(style.getColor(Style::Colors::BORDER_HOVERED));
    } else {
        batch.setColor(style.getColor(Style::Colors::BORDER));
    }
    batch.drawRectangle({inner_min.x, outer_min.y}, {inner_max.x, inner_min.y});
    batch.drawRectangle({inner_min.x, inner_max.y}, {inner_max.x, outer_max.y});
    batch.drawRectangle({outer_min.x, outer_min.y}, {inner_min.x, outer_max.y});
    batch.drawRectangle({inner_max.x, outer_min.y}, {outer_max.x, outer_max.y});

    if(m_widget.isDisabled()) {
        batch.setColor(style.getColor(Style::Colors::INPUT_DISABLED));
    } else if(m_widget.isSelected()) {
        batch.setColor(style.getColor(Style::Colors::INPUT_ACTIVE));
    } else if(m_widget.isHovered()) {
        batch.setColor(style.getColor(Style::Colors::INPUT_HOVERED));
    } else {
        batch.setColor(style.getColor(Style::Colors::INPUT));
    }
    batch.drawRectangle(inner_min, inner_max);
}

void TextInputWidget::View::drawText(
    RenderBatchInterface& batch, Style const& style, SdfFont const& font, vec2 origin, float scale
) {
    batch.setColor(style.getColor(Style::Colors::TEXT));
    for(auto const& [glyph, x] : m_glyphs) {
        batch.drawGlyph(glyph, origin + vec2{x * scale, 0}, scale);
    }
}

void TextInputWidget::View::drawCursor(
    RenderBatchInterface& batch,
    Style const&          style,
    SdfFont const&           font,
    vec2                  text_origin,
    float                 text_scale,
    usize                 cursor_pos
) {
    using namespace std::chrono;

    static const steady_clock::time_point ref_time = steady_clock::now();

    float time           = duration_cast<duration<float>>(steady_clock::now() - ref_time).count();
    float blink_duration = style.getValue(Style::Values::INPUT_CURSOR_BLINK_DURATION);
    float blink_on       = style.getValue(Style::Values::INPUT_CURSOR_BLINK_ON_DURATION);
    float dt             = fmod(time, blink_duration);

    if(dt > blink_on) return;

    float ascender  = font.getAscender();
    float descender = font.getDescender();
    batch.setColor(style.getColor(Style::Colors::TEXT));

    float x = text_origin.x
            + text_scale * (cursor_pos < m_glyphs.size() ? m_glyphs[cursor_pos].second : m_advance);
    float y_min = text_origin.y + text_scale * 0.9f * descender;
    float y_max = text_origin.y + text_scale * 0.9f * ascender;
    batch.drawRectangle({x - 0.5f, y_min}, {x + 0.5f, y_max});
}

} // namespace bembel::gui
