module;
#include "bembel/pch.h"
module bembel.gui.widgets;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

ButtonWidget::ButtonWidget(Widget& parent) : Widget{parent} {
    m_interaction_handles.push_back(&m_handle);

    size.change_signal.bind(this, &ButtonWidget::onSizeChanged);
    m_handle.press_signal.bind(this, &ButtonWidget::onPress);
    m_handle.release_signal.bind(this, &ButtonWidget::onRelease);

    m_handle.cursor = getGUI().getAssetManager().getAssetHandle<kernel::CursorIcon>("Hand");

    m_view = std::make_unique<SimpleButtonWidgetView>(*this);
}

ButtonWidget::~ButtonWidget() {}

bool ButtonWidget::configure(base::xml::Element const* properties) {
    Widget::configure(properties);
    // @ToDo configure View
    return true;
}

void ButtonWidget::onSizeChanged(In<ivec2>, In<ivec2> new_size) {
    m_handle.size = new_size;
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

inline Style::Colors getBorderColor(ButtonWidget& widget) {
    if(widget.isDisabled()) return Style::Colors::BUTTON_BORDER_DISABLED;
    if(widget.isPressed()) return Style::Colors::BUTTON_BORDER_ACTIVE;
    return Style::Colors(
        u32(Style::Colors::BUTTON_BORDER) + widget.isHovered() + 2 * widget.isSelected()
    );
}

inline Style::Colors getBackgroundColor(ButtonWidget& widget) {
    if(widget.isDisabled()) return Style::Colors::BUTTON_BACKGROUND_DISABLED;
    if(widget.isPressed()) return Style::Colors::BUTTON_BACKGROUND_ACTIVE;
    return Style::Colors(
        u32(Style::Colors::BUTTON_BACKGROUND) + widget.isHovered() + 2 * widget.isSelected()
    );
}

void SimpleButtonWidgetView::draw(RenderBatchInterface& batch) {
    auto style = m_button.getStyle();
    assert(style && "GUI::Style is undefined");

    float border = style->getValue(Style::Values::BUTTON_BORDER_WIDTH);

    vec2 outer_min = m_button.position.get();
    vec2 outer_max = outer_min + vec2(m_button.size.get());
    vec2 inner_min = outer_min + vec2(border);
    vec2 inner_max = outer_max - vec2(border);

    batch.setColor(style->getColor(getBorderColor(m_button)));
    batch.drawRectangle({inner_min.x, outer_min.y}, {inner_max.x, inner_min.y});
    batch.drawRectangle({inner_min.x, inner_max.y}, {inner_max.x, outer_max.y});
    batch.drawRectangle({outer_min.x, outer_min.y}, {inner_min.x, outer_max.y});
    batch.drawRectangle({inner_max.x, outer_min.y}, {outer_max.x, outer_max.y});

    batch.setColor(style->getColor(getBackgroundColor(m_button)));
    batch.drawRectangle(inner_min, inner_max);
}

} // namespace bembel::gui
