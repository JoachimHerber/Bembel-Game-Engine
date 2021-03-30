#ifndef BEMBEL_GUI_WIDGETS_WIDGETTYPEUTILS_HPP
#define BEMBEL_GUI_WIDGETS_WIDGETTYPEUTILS_HPP

#include <bembel/base/io/xml.hpp>
#include <bembel/base/utils/factory.hpp>

#include "./widget.hpp"

namespace bembel::gui {

namespace WidgetTypeUtils {

    using WidgetFactory = base::Factory<Widget, Widget*>;
    WidgetFactory& getWidgetFactory();

    template <class TWidgedType>
    void registerWidgetType(const std::string& type_name) {
        getWidgetFactory().registerObjectGenerator(type_name, [](Widget* parent_widget) {
            return std::make_unique<TWidgedType>(parent_widget->getGUI(), parent_widget);
        });
    }
}; // namespace WidgetTypeUtils

} // namespace bembel::gui
#endif // include guards
