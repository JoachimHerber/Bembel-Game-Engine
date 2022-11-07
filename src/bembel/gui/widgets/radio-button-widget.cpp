module;
#include "bembel/pch.h"
module bembel.gui.widgets;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

RadioButtonWidget::RadioButtonWidget(Widget& parent, int index) : Widget{parent}, m_index{index} {
    m_interaction_handles.push_back(&m_handle);
    m_child_widgets.push_back(&m_label);

    m_label.setName("Lable");
    m_label.alignment = LabelWidget::Alignment::Left;

    size.change_signal.bind(this, &RadioButtonWidget::onSizeChanged);
    m_handle.action_signal.bind(this, &RadioButtonWidget::onAction);

    m_view = std::make_unique<SimpleRadioButtonWidgetView>(*this);
}

bool RadioButtonWidget::configure(xml::Element const* properties) {
    return false;
}

ivec2 RadioButtonWidget::getMinSize() const {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float font_size = style->getValue(Style::Values::MIN_FONT_SIZE);
    float box_size  = style->getValue(Style::Values::CHECKBOX_SIZE);
    float margin    = style->getValue(Style::Values::CHECKBOX_LABLE_MARGIN);

    return {box_size + margin, std::max(box_size, font_size)};
}

void RadioButtonWidget::onSizeChanged(In<ivec2>, In<ivec2> new_size) {
    m_handle.size = new_size;

    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float box_size   = style->getValue(Style::Values::CHECKBOX_SIZE);
    float box_margin = style->getValue(Style::Values::CHECKBOX_LABLE_MARGIN);

    m_label.size     = ivec2{new_size.x - box_size - box_margin, new_size.y};
    m_label.position = ivec2{box_size + box_margin, 0};
}

void RadioButtonWidget::onAction(InteractionHandle::Action action, ivec2) {
    if(action == InteractionHandle::Action::INTERACT) { select(); }
}

void SimpleRadioButtonWidgetView::draw(RenderBatchInterface& batch) {
    auto style = m_widget.getStyle();
    assert(style && "GUI::Style is undefined");

    float size = style->getValue(Style::Values::CHECKBOX_SIZE);

    float border = style->getValue(Style::Values::INPUT_BORDER_WIDTH);
    float x      = m_widget.position.get().x;
    float y      = m_widget.position.get().y + 0.5f * (m_widget.size.get().y - size);

    auto tc =
        style->getTextureCoords(m_widget.isSelected() ? "radio_button_selected" : "radio_button");

    if(!tc) { return; }

    batch.setPrimaryColor(style->getColor(Style::Colors::INPUT_BORDER));
    batch.setSecondaryColor(style->getColor(Style::Colors::INPUT_BACKGROUND));
    batch.setAlpha(255);
    batch.drawIcon({x, y}, {x + size, y + size}, tc->min, tc->max);
}

RadioButtonGroupWidget::RadioButtonGroupWidget(Widget& parent) : Widget{parent} {
    size.change_signal.bind(this, &RadioButtonGroupWidget::onSizeChanged);
}

bool RadioButtonGroupWidget::configure(xml::Element const* properties) {
    return false;
}

ivec2 RadioButtonGroupWidget::getMinSize() const {
    ivec2 min_size{0, 0};
    for(auto& it : m_buttons) {
        ivec2 min = it->getMinSize();
        if(m_align_horizontal) {
            min_size.x += min.x;
            min_size.y += std::max(min_size.y, min.y);
        } else {
            min_size.x += std::max(min_size.x, min.x);
            min_size.y += min.y;
        }
    }
    return min_size;
}

void RadioButtonGroupWidget::addRadioButton(In<std::string_view> lable) {
    m_buttons.push_back(std::make_unique<RadioButtonWidget>(*this, int(m_buttons.size())));
    m_child_widgets.push_back(m_buttons.back().get());
    m_buttons.back()->text.set(std::string(lable));
    m_buttons.back()->select_signal.bind(this, &RadioButtonGroupWidget::setSelection);
}

int RadioButtonGroupWidget::getSelection() const {
    return m_selection;
}

void RadioButtonGroupWidget::setSelection(int index) {
    if(m_selection == index) return;

    m_selection = index;
    selection_change_signal.emit(m_selection);
    for(int i = 0; i < m_buttons.size(); ++i) {
        if(i == index)
            m_buttons[i]->select();
        else
            m_buttons[i]->deselect();
    }
}

void RadioButtonGroupWidget::onSizeChanged(In<ivec2>, In<ivec2> new_size) {
    if(m_buttons.empty()) return;

    ivec2 button_size   = new_size;
    ivec2 button_offset = m_align_horizontal ? ivec2{new_size.x / m_buttons.size(), 0}
                                             : ivec2{0, new_size.y / m_buttons.size()};
    if(m_align_horizontal) {
        button_size.x /= m_buttons.size();
    } else {
        button_size.y /= m_buttons.size();
    }
    ivec2 pos = {0, 0};

    for(auto& it : m_buttons) {
        it->size = button_size;
        it->position = pos;
        pos += button_offset;
    }
}

} // namespace bembel::gui