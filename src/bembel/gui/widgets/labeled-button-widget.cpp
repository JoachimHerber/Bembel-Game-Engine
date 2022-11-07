module;
#include "bembel/pch.h"
module bembel.gui.widgets;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

LabeledButtonWidget::LabeledButtonWidget(Widget& parent) : Widget{parent} {
    m_child_widgets.push_back(&m_button);
    m_child_widgets.push_back(&m_label);

    m_button.setName("Button");
    m_label.setName("Lable");

    this->size.change_signal.bind(this, &LabeledButtonWidget::onSizeChanged);

    auto style = parent.getStyle();
    if(style) {
        float border     = style->getValue(Style::Values::BUTTON_BORDER_WIDTH);
        m_label.position = ivec2(border, border);
    }
}

bool LabeledButtonWidget::configure(xml::Element const* properties) {
    Widget::configure(properties);

    xml::getAttribute(properties, "label", this->text);
    return true;
}

void LabeledButtonWidget::onSizeChanged(In<ivec2>, In<ivec2> new_size) {
    auto style = this->getStyle();
    assert(style && "GUI::Style is undefined");

    float border = style->getValue(Style::Values::BUTTON_BORDER_WIDTH);

    m_button.size = new_size;
    m_label.size  = new_size - glm::ivec2(2 * border, 2 * border);
}

} // namespace bembel::gui
