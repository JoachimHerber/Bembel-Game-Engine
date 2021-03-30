#include "./position-component.hpp"

namespace bembel::kernel {

const std::string& PositionComponent::getComponentTypeName() {
    const static std::string type_name = "Position";
    return type_name;
}

bool PositionComponent::initComponent(
    AssetManager&, const base::xml::Element* properties, PositionComponent* component) {
    xml::getAttribute(properties, "x", component->position.x);
    xml::getAttribute(properties, "y", component->position.y);
    xml::getAttribute(properties, "z", component->position.z);
    return true;
}

} // namespace bembel::kernel
