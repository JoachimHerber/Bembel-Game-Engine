module;
#include <string_view>
export module bembel.kernel.core:Transform;

import bembel.base;
import bembel.kernel.assets;

import :Components;

namespace bembel::kernel {
using namespace bembel::base;

/// This Component stores the 3D position, roation and scale of an entity
export struct Transform {
    vec3  position = {0.f, 0.f, 0.f};
    float scale    = 1.f;
    quat  rotation = {1.f, 0.f, 0.f, 0.f};

    using Container = FixedAddressComponentVector<Transform>;

    static bool deserialize(Container* container, EntityID entity_id, xml::Element const* entity) {
        auto* properties = entity->FirstChildElement("Transform");
        if(!properties) return false;

        vec3  position = {0.f, 0.f, 0.f};
        float scale    = 1.f;
        quat  rotation = {1.f, 0.f, 0.f, 0.f};

        if(!xml::getAttribute(properties, "position", position)) {
            xml::getAttribute(properties, "x", position.x);
            xml::getAttribute(properties, "y", position.y);
            xml::getAttribute(properties, "z", position.z);
        }

        container->assignComponent(entity_id, position, scale, rotation);
        return true;
    }
};

} // namespace bembel::kernel
