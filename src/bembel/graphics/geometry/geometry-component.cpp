module;
#include <string>
#include <string_view>
module bembel.graphics.geometry;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

bool Geometry::deserialize(Container* container, EntityID entity_id, xml::Element const* entity) {
    auto* properties = entity->FirstChildElement("Geometry");
    if(!properties) return false;

    std::string model_name;
    if(!xml::getAttribute(properties, "model", model_name)) return false;

    vec3 scale = {1, 1, 1};
    xml::getAttribute(properties, "scale", scale);

    container->assignComponent(entity_id, Asset<GeometryModel>(model_name), scale);
    return true;
}

} // namespace bembel::graphics
