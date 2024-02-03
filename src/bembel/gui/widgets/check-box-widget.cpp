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

CheckBoxWidget::CheckBoxWidget(In<Widget*> parent, std::u8string_view label)
  : Widget{parent}, m_label{this, label} {
    m_interaction_handles.push_back(&m_handle);
    m_child_widgets.push_back(&m_label);

    m_label.setName("Lable");

    size.change_signal.bind(this, &CheckBoxWidget::onSizeChanged);
    m_handle.action_signal.bind(this, &CheckBoxWidget::onAction);

    m_view = std::make_unique<SimpleCheckBoxWidgetView>(this);
}

bool CheckBoxWidget::configure(xml::Element const* properties) {
    return false;
}

uint CheckBoxWidget::getMinWidth(In<std::optional<uint>> height) const {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float box_size = style->getValue(Style::Values::CHECKBOX_SIZE);
    float margin   = style->getValue(Style::Values::CHECKBOX_LABLE_MARGIN);

    return box_size + margin + m_label.getMinWidth(height);
}

uint CheckBoxWidget::getMinHeight(In<std::optional<uint>> width) const {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    float box_size = style->getValue(Style::Values::CHECKBOX_SIZE);
    float margin   = style->getValue(Style::Values::CHECKBOX_LABLE_MARGIN);

    return std::max(uint(box_size), m_label.getMinHeight(width.and_then([=](uint w) {
        return std::optional<uint>(std::max(w, uint(box_size + margin)) - uint(box_size + margin));
    })));
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

void SimpleCheckBoxWidgetView::draw(InOut<RenderBatchInterface> batch) {
    auto style = m_widget->getStyle();
    assert(style && "GUI::Style is undefined");

    float size = style->getValue(Style::Values::CHECKBOX_SIZE);

    float border = style->getValue(Style::Values::INPUT_BORDER_WIDTH);
    float x      = m_widget->position.get().x;
    float y      = m_widget->position.get().y + 0.5f * (m_widget->size.get().y - size);

    auto tc = style->getTextureCoords([&]() {
        switch(m_widget->state.get()) {
            case CheckBoxWidget::State::UNSELECTED: return "check_box";
            case CheckBoxWidget::State::SELECTED: return "check_box_selected";
            case CheckBoxWidget::State::INDETERMINATE: return "check_box_indeterminate";
        }
        return "";
    }());

    if(!tc) { return; }

    if(m_widget->isDisabled()) {
        batch.setPrimaryColor(style->getColor(Style::Colors::CHECKBOX_DISABLED));
        batch.setSecondaryColor(style->getColor(Style::Colors::BORDER_DISABLED));
    } else if(m_widget->isHovered()) {
        batch.setPrimaryColor(style->getColor(Style::Colors::CHECKBOX_HOVERED));
        batch.setSecondaryColor(style->getColor(Style::Colors::BORDER_HOVERED));
    } else {
        batch.setPrimaryColor(style->getColor(Style::Colors::CHECKBOX));
        batch.setSecondaryColor(style->getColor(Style::Colors::BORDER));
    }
    batch.setAlpha(255);
    batch.drawIcon({x, y}, {x + size, y + size}, tc->min, tc->max);
}

} // namespace bembel::gui
