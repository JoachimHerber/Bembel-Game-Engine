#include "./light-source.hpp"

#include <bembel/kernel/scene/position-component.hpp>

namespace bembel::graphics {

const std::string& PointLightSource::getComponentTypeName() {
    const static std::string typeName = "PointLight";
    return typeName;
}

bool PointLightSource::initComponent(
    kernel::AssetManager&, const base::xml::Element* properties, PointLightSource* component) {
    component->bulb_radius = 1;
    base::xml::getAttribute(properties, "color", component->color);
    float intensity;
    if(base::xml::getAttribute(properties, "intensity", intensity)) component->color *= intensity;

    base::xml::getAttribute(properties, "bulbRadius", component->bulb_radius);
    base::xml::getAttribute(properties, "cutoffRadius", component->cutoff_radius);

    return true;
}

const std::string& DirectionalLightSource::getComponentTypeName() {
    const static std::string typeName = "DirectionalLight";
    return typeName;
}

bool DirectionalLightSource::initComponent(
    kernel::AssetManager&,
    const base::xml::Element* properties,
    DirectionalLightSource*   component) {
    base::xml::getAttribute(properties, "color", component->color);
    float intensity;
    if(base::xml::getAttribute(properties, "intensity", intensity)) component->color *= intensity;
    base::xml::getAttribute(properties, "direction", component->direction);
    component->direction = glm::normalize(component->direction);

    return true;
}

} // namespace bembel::graphics
