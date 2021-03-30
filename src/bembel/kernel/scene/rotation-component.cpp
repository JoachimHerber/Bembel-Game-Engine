#include "./rotation-component.hpp"

namespace bembel::kernel {

const std::string& RotationComponent::getComponentTypeName() {
    const static std::string type_name = "Rotation";
    return type_name;
}

bool RotationComponent::initComponent(
    AssetManager&, const base::xml::Element* properties, RotationComponent* component) {
    glm::vec3 axis;
    float     angle;
    if(xml::getAttribute(properties, "axis", axis)
       && xml::getAttribute(properties, "angle", angle)) {
        angle               = glm::radians(angle);
        component->rotation = glm::angleAxis(angle, axis);
    }
    return true;
}

} // namespace bembel::kernel
