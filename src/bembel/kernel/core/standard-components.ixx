module;
#include "bembel/pch.h"
export module bembel.kernel.core:StandardComponents;

import bembel.base;
import bembel.kernel.assets;

import :Components;

namespace bembel::kernel {
using namespace bembel::base;

/// This Component stores the 3D position of an entity
export struct PositionComponent {
  public:
    vec3 position;

    PositionComponent& operator=(In<vec3> pos) {
        position = pos;
        return *this;
    }
    operator vec3&() { return position; }
    operator vec3 const&() const { return position; }

    static constexpr std::string_view COMPONENT_TYPE_NAME = "Position";

    static bool initComponent(
        In<xml::Element const*> properties, InOut<PositionComponent> component
    ) {
        return xml::getAttribute(properties, "x", component.position.x)
            && xml::getAttribute(properties, "y", component.position.y)
            && xml::getAttribute(properties, "z", component.position.z);
    }

    using ContainerType = ComponentArray<PositionComponent>;
};

/// This Component stores the 3D rotation of an entity
export struct RotationComponent {
    quat rotation;

    RotationComponent& operator=(In<quat> rot) {
        rotation = rot;
        return *this;
    }
    operator quat&() { return rotation; }
    operator quat const&() const { return rotation; }

    static constexpr std::string_view COMPONENT_TYPE_NAME = "Rotation";

    static bool initComponent(
        In<xml::Element const*> properties, InOut<RotationComponent> component
    ) {
        return xml::getAttribute(properties, "x", component.rotation.x)
            && xml::getAttribute(properties, "y", component.rotation.y)
            && xml::getAttribute(properties, "z", component.rotation.z)
            && xml::getAttribute(properties, "w", component.rotation.w);
    }

    using ContainerType = ComponentArray<RotationComponent>;
};

} // namespace bembel::kernel
