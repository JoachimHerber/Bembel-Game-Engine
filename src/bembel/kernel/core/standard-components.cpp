module;
#include "bembel/pch.h"
module bembel.kernel.core;

import bembel.base;

namespace bembel::kernel {
using namespace bembel::base;



bool PositionComponent::initComponent(
    AssetManager&, const xml::Element* properties, PositionComponent& component) {
    xml::getAttribute(properties, "x", component.position.x);
    xml::getAttribute(properties, "y", component.position.y);
    xml::getAttribute(properties, "z", component.position.z);
    return true;
}


bool RotationComponent::initComponent(
    AssetManager&, const xml::Element* properties, RotationComponent& component) {
    glm::vec3 axis;
    float     angle;
    if(xml::getAttribute(properties, "axis", axis)
       && xml::getAttribute(properties, "angle", angle)) {
        angle              = glm::radians(angle);
        component.rotation = glm::angleAxis(angle, axis);
    }
    return true;
}

} // namespace bembel::kernel
