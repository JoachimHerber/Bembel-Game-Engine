module;
#include <cassert>
#include <memory>
#include <string_view>
module bembel.gui.widgets;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

TextWidget::TextWidget(Widget& parent) : Widget{parent} {
    text.change_signal.bind(this, &TextWidget::onTextChanged);
    size.change_signal.bind(this, &TextWidget::onSizeChanged);
    m_view = std::make_unique<TextWidget::View>(*this);
}

bool TextWidget::configure(xml::Element const* properties) {
    Widget::configure(properties);
    if(!properties) return true;

    auto content = properties->FirstChildElement("Content");
    if(content) {
        String text = std::u8string_view((char8_t*)content->GetText());
        this->text.set(text);
    }

    xml::getAttribute(properties, "FontSize", m_fontSize);

    m_view = std::make_unique<TextWidget::View>(*this);
    return true;
}

uint TextWidget::getMinWidth() const {
    return 0;
}

uint TextWidget::getMinHeight() const {
    return 0;
}

void TextWidget::onTextChanged(In<String>, In<String> new_text) {
    auto style = this->getStyle();
    assert(style && "GUI::Style is undefined");
    auto font = style->getFont();
    assert(font && "Font is undefined");

    m_text.setFont(font);
    m_text.parse(new_text);

    recalculateLayout(size.get());
}

void TextWidget::onSizeChanged(In<ivec2>, In<ivec2> new_size) {
    recalculateLayout(new_size);
}

void TextWidget::recalculateLayout(In<ivec2> size) {
    Font const* const font = m_text.getFont();
    if(!font) return;

    vec2 origin = {0, size.y - m_fontSize * font->getAscender()};

    m_layout.calculateSimpleLayout(m_text, origin, m_fontSize, size.x);
}

void TextWidget::View::draw(RenderBatchInterface& batch) {
    auto style = m_widget.getStyle();
    assert(style && "GUI::Style is undefined");
    auto font = style->getFont();
    assert(font && "Font is undefined");

    glm::vec2 size     = m_widget.size.get();
    glm::vec2 position = m_widget.position.get();

    // batch.setColor(style->getColor(Style::Colors::TEXT_BACKGROUND));
    // batch.drawRectangle(position, position + size);

    batch.setColor(style->getColor(Style::Colors::TEXT_OUTLINE));
    for(auto& g : m_widget.m_layout.getGlyphs()) {
        batch.drawGlyph(*g.glyph, g.pos + position, m_widget.m_fontSize, 0.15f, 0.20f);
    }
    if(m_widget.text_color.has_value()) {
        batch.setColor(m_widget.text_color.value());
    } else {
        batch.setColor(style->getColor(Style::Colors::TEXT));
    }

    for(auto& g : m_widget.m_layout.getGlyphs()) {
        batch.drawGlyph(*g.glyph, g.pos + position, m_widget.m_fontSize);
    }
}

} // namespace bembel::gui
