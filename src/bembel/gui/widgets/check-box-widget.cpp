module;
#include "bembel/pch.h"
module bembel.gui.widgets;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

CheckBoxWidget::CheckBoxWidget(Widget& parent) : Widget{parent} {
    m_interaction_handles.push_back(&m_handle);
    m_child_widgets.push_back(&m_label);

    m_label.setName("Lable");
    m_label.alignment = LabelWidget::Alignment::Left;

    size.change_signal.bind(this, &CheckBoxWidget::onSizeChanged);
    m_handle.action_signal.bind(this, &CheckBoxWidget::onAction);

    m_view = std::make_unique<SimpleCheckBoxWidgetView>(*this);
}

bool CheckBoxWidget::configure(xml::Element const* properties) {
    return false;
}

ivec2 CheckBoxWidget::getMinSize() const {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float font_size = style->getValue(Style::Values::MIN_FONT_SIZE);
    float box_size  = style->getValue(Style::Values::CHECKBOX_SIZE);
    float margin    = style->getValue(Style::Values::CHECKBOX_LABLE_MARGIN);

    return {box_size + margin, std::max(box_size, font_size)};
}

void CheckBoxWidget::onSizeChanged(In<ivec2>, In<ivec2> new_size) {
    m_handle.size = new_size;

    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float box_size   = style->getValue(Style::Values::CHECKBOX_SIZE);
    float box_margin = style->getValue(Style::Values::CHECKBOX_LABLE_MARGIN);

    m_label.size     = ivec2{new_size.x - box_size - box_margin, new_size.y};
    m_label.position = ivec2{box_size + box_margin, 0};
}

void CheckBoxWidget::onAction(InteractionHandle::Action action, ivec2) {
    if(action == InteractionHandle::Action::INTERACT) {
        this->state.set(this->state.get() == State::SELECTED ? State::UNSELECTED : State::SELECTED);
    }
}

void SimpleCheckBoxWidgetView::draw(RenderBatchInterface& batch) {
    auto style = m_widget.getStyle();
    assert(style && "GUI::Style is undefined");

    float size = style->getValue(Style::Values::CHECKBOX_SIZE);

    float border = style->getValue(Style::Values::INPUT_BORDER_WIDTH);
    float x      = m_widget.position.get().x;
    float y      = m_widget.position.get().y + 0.5f * (m_widget.size.get().y - size);

    auto tc = style->getTextureCoords([&]() {
        switch(m_widget.state.get()) {
            case CheckBoxWidget::State::UNSELECTED: return "check_box";
            case CheckBoxWidget::State::SELECTED: return "check_box_selected";
            case CheckBoxWidget::State::INDETERMINATE: return "check_box_indeterminate";
        }
        return "";
    }());

    if(!tc) { return; }

    batch.setPrimaryColor(style->getColor(
        m_widget.isDisabled() ? Style::Colors::CHECKBOX_BACKGROUND_DISABLED
                              : Style::Colors::CHECKBOX_BACKGROUND
    ));
    batch.setSecondaryColor(style->getColor(Style::Colors::INPUT_BACKGROUND));
    batch.setAlpha(255);
    batch.drawIcon({x, y}, {x + size, y + size}, tc->min, tc->max);
}

} // namespace bembel::gui