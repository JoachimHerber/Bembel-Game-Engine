#include "./widget-type-utils.hpp"

namespace bembel::gui {
namespace WidgetTypeUtils {

    WidgetFactory& getWidgetFactory() {
        static WidgetFactory factory;
        return factory;
    }

}; // namespace WidgetTypeUtils
} // namespace bembel::gui
