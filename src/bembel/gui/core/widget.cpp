module;
#include "bembel/pch.h"
module bembel.gui.core;

import bembel.base;
import bembel.kernel;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

bool Widget::configure(xml::Element const* properties) {
    if(properties) {
        xml::getAttribute(properties, "name", m_name);
        xml::getAttribute(properties, "position", this->position);
        xml::getAttribute(properties, "size", this->size);
    }
    return true;
}

void Widget::show() {
    m_hidden = false;
}

void Widget::hide() {
    m_hidden = true;
}

Style* Widget::getStyle() const {
    return m_gui.getRenderer().getStyle();
}

Widget* Widget::getChildWidget(std::string_view path) const {
    auto delim_pos   = path.find('/');
    auto widget_name = delim_pos == path.npos ? path : path.substr(0, delim_pos);

    for(Widget* widget : m_child_widgets) {
        if(widget->getName() != widget_name) continue;

        if(delim_pos == path.npos) {
            return widget;
        } else {
            return widget->getChildWidget(path.substr(delim_pos + 1));
        }
    }

    return nullptr;
}

Factory<Widget, Widget&>& Widget::getFactory() {
    static Factory<Widget, Widget&> factory;
    return factory;
}

} // namespace bembel::gui
