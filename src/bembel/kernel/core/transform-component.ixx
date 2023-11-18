module;
#include <string_view>
export module bembel.kernel.core:Transform;

import bembel.base;
import bembel.kernel.assets;

import :Components;

namespace bembel::kernel {
using namespace bembel::base;

/// This Component stores the 3D position, roation and scale of an entity
export struct Transform{
    //static constexpr std::string_view COMPONENT_TYPE_NAME = "Transform";
    //
    //using Container = FixedAddressComponentVector<Transform>;

    vec3  position = {0.f, 0.f, 0.f};
    float scale    = 1.f;
    quat  rotation = {1.f, 0.f, 0.f, 0.f};
};

export bool initComponent(xml::Element const* properties, Transform& transform) {
    if(!xml::getAttribute(properties, "position", transform.position)) {
        xml::getAttribute(properties, "x", transform.position.x);
        xml::getAttribute(properties, "y", transform.position.y);
        xml::getAttribute(properties, "z", transform.position.z);
    }

    // xml::getAttribute(properties, "rotation", transform.rotation);
    return true;
}

export template <>
struct ComponentMetaData<Transform>
  : BasicComponentMetaData<"Transform", Transform, ComponentContainer::FIXED_ADDRESS_VECTOR> {};

} // namespace bembel::kernel
