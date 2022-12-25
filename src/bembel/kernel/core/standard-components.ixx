module;
#include <string_view>
export module bembel.kernel.core:StandardComponents;

import bembel.base;
import bembel.kernel.assets;

import :Components;

namespace bembel::kernel {
using namespace bembel::base;

export struct TransformationData {
    vec3  position = {0.f, 0.f, 0.f};
    float scale    = 1.f;
    quat  rotation = {1.f, 0.f, 0.f, 0.f};
};

export bool initComponent(
    xml::Element const* properties, AssetManager& asset_mgr, TransformationData& transform
) {
    if(!xml::getAttribute(properties, "position", transform.position)) {
        xml::getAttribute(properties, "x", transform.position.x);
        xml::getAttribute(properties, "y", transform.position.y);
        xml::getAttribute(properties, "z", transform.position.z);
    }

    //xml::getAttribute(properties, "rotation", transform.rotation);
    return true;
}

/// This Component stores the 3D position, roation and scale of an entity
export using Transform =
    BasicComponent<"Transform", TransformationData, ComponentContainer::FIXED_ADDRESS_VECTOR>;

} // namespace bembel::kernel
