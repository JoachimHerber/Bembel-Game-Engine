module;
#include <string>
#include <string_view>
module bembel.graphics.geometry;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

bool initComponent(
    In<xml::Element const*> properties, InOut<GeometryComponentData> geometry_component
) {
    std::string model_name;
    if(!xml::getAttribute(properties, "model", model_name)) return false;

    xml::getAttribute(properties, "scale", geometry_component.scale);

    geometry_component.model = Asset<GeometryModel>(model_name);
    return geometry_component.model;
}

} // namespace bembel::graphics
