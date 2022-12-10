module;
#include "bembel/pch.h"
module bembel.graphics.pipeline;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

bool initComponent(
    In<xml::Element const*> properties,
    InOut<AssetManager>     asset_mgr,
    InOut<PointLightSource> component
) {
    component.bulb_radius = 1;
    xml::getAttribute(properties, "color", component.color);
    float intensity;
    if(xml::getAttribute(properties, "intensity", intensity)) component.color *= intensity;

    xml::getAttribute(properties, "bulbRadius", component.bulb_radius);
    xml::getAttribute(properties, "cutoffRadius", component.cutoff_radius);

    return true;
}

bool initComponent(
    In<xml::Element const*>       properties,
    InOut<AssetManager>           asset_mgr,
    InOut<DirectionalLightSource> component
) {
    xml::getAttribute(properties, "color", component.color);
    float intensity;
    if(base::xml::getAttribute(properties, "intensity", intensity)) component.color *= intensity;
    xml::getAttribute(properties, "direction", component.direction);
    component.direction = glm::normalize(component.direction);

    return true;
}

} // namespace bembel::graphics
