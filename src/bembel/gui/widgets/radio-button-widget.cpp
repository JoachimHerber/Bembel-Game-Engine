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

RadioButtonWidget::RadioButtonWidget(Widget& parent, int index) : Widget{parent}, m_index{index} {
    m_interaction_handles.push_back(&m_handle);
    m_child_widgets.push_back(&m_label);

    m_label.setName("Lable");

    size.change_signal.bind(this, &RadioButtonWidget::onSizeChanged);
    m_handle.action_signal.bind(this, &RadioButtonWidget::onAction);

    m_view = std::make_unique<SimpleRadioButtonWidgetView>(*this);
}

bool RadioButtonWidget::configure(xml::Element const* properties) {
    return false;
}

uint RadioButtonWidget::getMinWidth(In<std::optional<uint>> height) const {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    uint box_size = style->getValue(Style::Values::CHECKBOX_SIZE);
    uint margin   = style->getValue(Style::Values::CHECKBOX_LABLE_MARGIN);

    return box_size + margin + m_label.getMinWidth(height);
}

uint RadioButtonWidget::getMinHeight(In<std::optional<uint>> width) const {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    uint box_size = style->getValue(Style::Values::CHECKBOX_SIZE);
    uint margin   = style->getValue(Style::Values::CHECKBOX_LABLE_MARGIN);

    return std::max(box_size, m_label.getMinHeight(width.and_then([=](uint w) {
        return std::optional<uint>(std::max(w, box_size + margin) - (box_size + margin));
    })));
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

    batch.setPrimaryColor(style->getColor(Style::Colors::INPUT));
    batch.setSecondaryColor(style->getColor(Style::Colors::BORDER));
    batch.setAlpha(255);
    batch.drawIcon({x, y}, {x + size, y + size}, tc->min, tc->max);
}

RadioButtonGroupWidget::RadioButtonGroupWidget(Widget& parent) : Widget{parent} {
    size.change_signal.bind(this, &RadioButtonGroupWidget::onSizeChanged);
}

bool RadioButtonGroupWidget::configure(xml::Element const* properties) {
    return false;
}

uint RadioButtonGroupWidget::getMinWidth(In<std::optional<uint>> height) const {
    if(m_buttons.empty()) return 0;

    uint min_width = 0;
    for(auto& button : m_buttons) min_width = std::max(min_width, button->getMinWidth({}));

    if(height) {
        uint min_height = 0;
        for(auto& button : m_buttons) min_height = std::max(min_height, button->getMinHeight({}));
        uint num_rows = std::max<uint>(1, *height / min_height);
        uint num_collums =
            std::max<uint>(1, m_buttons.size() / num_rows + (m_buttons.size() % num_rows ? 1 : 0));

        min_width *= num_collums;
    }

    return min_width;
}

uint RadioButtonGroupWidget::getMinHeight(In<std::optional<uint>> width) const {
    if(m_buttons.empty()) return 0;

    uint min_height = 0;
    for(auto& button : m_buttons) min_height = std::max(min_height, button->getMinHeight({}));

    if(width) {
        uint min_width = 0;
        for(auto& button : m_buttons) min_width = std::max(min_width, button->getMinWidth({}));

        uint num_collums = std::max<uint>(1, *width / min_width);
        uint num_rows = m_buttons.size() / num_collums + (m_buttons.size() % num_collums ? 1 : 0);

        min_height *= num_rows;
    }
    return min_height;
}

void RadioButtonGroupWidget::addRadioButton(In<std::u8string_view> lable) {
    m_buttons.push_back(std::make_unique<RadioButtonWidget>(*this, int(m_buttons.size())));
    m_child_widgets.push_back(m_buttons.back().get());
    m_buttons.back()->setText(lable);
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

    uint max_button_width = 0;
    for(auto& button : m_buttons)
        max_button_width = std::max(max_button_width, button->getMinWidth({}));

    uint num_collums =
        std::max<uint>(1, std::min<uint>(new_size.x / max_button_width, m_buttons.size()));
    uint num_rows    = m_buttons.size() / num_collums + (m_buttons.size() % num_collums ? 1 : 0);

    ivec2 button_size = {new_size.x / num_collums, new_size.y / num_rows};
    int   y           = new_size.y;
    int   x           = 0;

    for(uint n = 0; n < m_buttons.size(); ++n) {
        if(n % num_collums == 0) {
            x = 0;
            y -= button_size.y;
        }
        m_buttons[n]->size     = button_size;
        m_buttons[n]->position = ivec2{x, y};
        x += button_size.x;
    }
}

} // namespace bembel::gui
