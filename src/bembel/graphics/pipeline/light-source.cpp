module;
#include <glm/glm.hpp>
#include <memory>
#include <string_view>
#include <vector>
module bembel.graphics.pipeline;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

bool PointLight::deserialize(Container* container, EntityID entity_id, xml::Element const* entity) {
    auto* properties = entity->FirstChildElement("PointLight");
    if(!properties) return false;

    vec3 color;
    xml::getAttribute(properties, "color", color);
    float intensity;
    if(xml::getAttribute(properties, "intensity", intensity)) color *= intensity;

    float bulb_radius;
    xml::getAttribute(properties, "bulbRadius", bulb_radius);
    float cutoff_radius;
    xml::getAttribute(properties, "cutoffRadius", cutoff_radius);

    container->assignComponent(entity_id, color, bulb_radius, cutoff_radius);
    return true;
}

bool DirectionalLight::deserialize(
    Container* container, EntityID entity_id, xml::Element const* entity
) {
    auto* properties = entity->FirstChildElement("DirectionalLight");
    if(!properties) return false;

    auto color       = xml::getAttribute<vec3>(properties, "color");
    auto intensity   = xml::getAttribute<float>(properties, "intensity");
    auto direction   = xml::getAttribute<vec3>(properties, "direction");
    auto cast_shadow = xml::getAttribute<bool>(properties, "cast_shadow");

    container->assignComponent(
        entity_id,
        color.value_or(vec3{1, 1, 1}) * intensity.value_or(1.0f),
        glm::normalize(direction.value_or(vec3{0, -1, 0})),
        cast_shadow.value_or(false)
    );
    return true;
}

} // namespace bembel::graphics