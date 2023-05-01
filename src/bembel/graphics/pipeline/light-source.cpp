module;
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <string_view>
module bembel.graphics.pipeline;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

bool initComponent(In<xml::Element const*> properties, InOut<PointLightData> component) {
    xml::getAttribute(properties, "color", component.color);
    float intensity;
    if(xml::getAttribute(properties, "intensity", intensity)) component.color *= intensity;

    xml::getAttribute(properties, "bulbRadius", component.bulb_radius);
    xml::getAttribute(properties, "cutoffRadius", component.cutoff_radius);

    return true;
}

bool initComponent(In<xml::Element const*> properties, InOut<DirectionalLightData> component) {
    xml::getAttribute(properties, "color", component.color);
    float intensity;
    if(xml::getAttribute(properties, "intensity", intensity)) component.color *= intensity;
    xml::getAttribute(properties, "direction", component.direction);
    component.direction = glm::normalize(component.direction);

    xml::getAttribute(properties, "cast_shadow", component.cast_shadow);

    return true;
}

} // namespace bembel::graphics
