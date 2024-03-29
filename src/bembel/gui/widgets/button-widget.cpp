﻿module;
#include <cassert>
#include <cctype>
#include <memory>
#include <string_view>
module bembel.gui.widgets;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

ButtonWidget::ButtonWidget(In<Widget*> parent, std::u8string_view text)
  : Widget{parent}, m_label{this, text} {
    m_interaction_handles.push_back(&m_handle);
    m_child_widgets.push_back(&m_label);

    m_label.setName("Lable");
    m_label.setAlignment(LabelWidget::Alignment::Center);

    size.change_signal.bind(this, &ButtonWidget::onSizeChanged);
    m_handle.press_signal.bind(this, &ButtonWidget::onPress);
    m_handle.release_signal.bind(this, &ButtonWidget::onRelease);

    m_handle.cursor = Asset<CursorIcon>("Hand");

    m_view = std::make_unique<SimpleButtonWidgetView>(this);
}

ButtonWidget::~ButtonWidget() {}

bool ButtonWidget::configure(base::xml::Element const* properties) {
    Widget::configure(properties);
    std::u8string label;
    if(xml::getAttribute(properties, "label", label)) { m_label.setText(label); }
    // @ToDo configure View
    return true;
}

uint ButtonWidget::getMinWidth(In<std::optional<uint>> height) const {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    uint margin = 2 * style->getValue(Style::Values::BUTTON_TEXT_MARGIN);

    return margin + m_label.getMinWidth(height.and_then([=](uint h) {
        return std::optional<uint>(std::min(h, margin) - margin);
    }));
}

uint ButtonWidget::getMinHeight(In<std::optional<uint>> width) const {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    uint margin = 2 * style->getValue(Style::Values::BUTTON_TEXT_MARGIN);

    return margin + m_label.getMinHeight(width.and_then([=](uint w) {
        return std::optional<uint>(std::min(w, margin) - margin);
    }));
}

void ButtonWidget::onSizeChanged(In<ivec2>, In<ivec2> new_size) {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    m_handle.size = new_size;

    int margin = int(style->getValue(Style::Values::BUTTON_TEXT_MARGIN));

    m_label.size     = new_size - 2 * ivec2{margin};
    m_label.position = ivec2{margin};
}

void ButtonWidget::onPress(ivec2) {
    m_is_pressed = true;
    press_signal.emit();
}

void ButtonWidget::onRelease(ivec2) {
    m_is_pressed = false;
    release_signal.emit();

    if(isHovered()) click_signal.emit();
}

inline Style::Colors getBorderColor(In<ButtonWidget> widget) {
    if(widget.isPressed()) return Style::Colors::BORDER_ACTIVE;
    if(widget.isHovered() || widget.isSelected()) return Style::Colors::BORDER_HOVERED;
    return Style::Colors::BORDER;
}

inline Style::Colors getButtonColor(In<ButtonWidget> widget) {
    if(widget.isDisabled()) return Style::Colors::BUTTON_DISABLED;
    if(widget.isPressed()) return Style::Colors::BUTTON_ACTIVE;
    if(widget.isHovered() || widget.isSelected()) return Style::Colors::BUTTON_HOVERED;
    return Style::Colors::BUTTON;
}

void SimpleButtonWidgetView::draw(InOut<RenderBatchInterface> batch) {
    auto style = m_button->getStyle();
    assert(style && "GUI::Style is undefined");
    auto font = style->getFont();
    assert(font && "Font is undefined");

    auto tc = style->getTextureCoords("button");
    if(!tc) { return; }

    float const u0 = tc->min.x;
    float const u1 = (tc->min.x + tc->max.x) * 0.5f;
    float const u2 = tc->max.x;

    float const v0 = tc->min.y;
    float const v1 = (tc->min.y + tc->max.y) * 0.5f;
    float const v2 = tc->max.y;

    float const corner_radius = style->getValue(Style::Values::BUTTON_CORNER_SIZE);

    float const x0 = m_button->position.get().x;
    float const x1 = x0 + corner_radius;
    float const x3 = x0 + m_button->size.get().x;
    float const x2 = x3 - corner_radius;

    float const y0 = m_button->position.get().y;
    float const y1 = y0 + corner_radius;
    float const y3 = y0 + m_button->size.get().y;
    float const y2 = y3 - corner_radius;

    batch.setPrimaryColor(style->getColor(getButtonColor(m_button)));
    batch.setSecondaryColor(style->getColor(getBorderColor(m_button)));
    batch.setAlpha(0xff);

    batch.drawIcon({x0, y0}, {x1, y1}, {u0, v0}, {u1, v1});
    batch.drawIcon({x1, y0}, {x2, y1}, {u1, v0}, {u1, v1});
    batch.drawIcon({x2, y0}, {x3, y1}, {u1, v0}, {u2, v1});
    batch.drawIcon({x0, y1}, {x1, y2}, {u0, v1}, {u1, v1});
    batch.drawIcon({x1, y1}, {x2, y2}, {u1, v1}, {u1, v1});
    batch.drawIcon({x2, y1}, {x3, y2}, {u1, v1}, {u2, v1});
    batch.drawIcon({x0, y2}, {x1, y3}, {u0, v1}, {u1, v2});
    batch.drawIcon({x1, y2}, {x2, y3}, {u1, v1}, {u1, v2});
    batch.drawIcon({x2, y2}, {x3, y3}, {u1, v1}, {u2, v2});
}
} // namespace bembel::gui
