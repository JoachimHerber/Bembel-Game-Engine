module;
#include "bembel/pch.h"
module bembel.gui.core;

import bembel.base;
import bembel.kernel;

namespace bembel::gui::WidgetTypeUtils {
using namespace bembel::base;
using namespace bembel::kernel;

std::unique_ptr<Widget> createWidget(
    std::string_view widget_type, Widget& parent, base::xml::Element const* properties) {
    auto widget = getWidgetFactory().createObject(widget_type, parent);

    if(!widget) {
        Log().warning("Can't create Widget of type '{}'", widget_type);
        Log().warning("\tType has not been registerd in WidgetFactory");
        return {};
    }
    if(properties) {
        if(!widget->configure(properties)) return {};
    }

    return widget;
}

} // namespace bembel::gui::WidgetTypeUtils
