module;
#include "bembel/pch.h"
export module bembel.gui.core : WidgetTypeUtils;

import bembel.base;
import bembel.kernel;
import : Widget;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export namespace WidgetTypeUtils {


    template <class TWidgedType>
    bool registerWidgetType() {
        return getWidgetFactory().registerObjectGenerator<TWidgedType>(
            TWidgedType::WIDGET_TYPE_NAME);
    }

    std::unique_ptr<Widget> createWidget(std::string_view widget_type, Widget& parent, xml::Element const* properties);
}; // namespace WidgetTypeUtils

} // namespace bembel::gui
