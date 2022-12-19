module;
#include "bembel/pch.h"
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
    vec3 pos = {0, 0, 0};
    if(!xml::getAttribute(properties, "position", pos)) {
        xml::getAttribute(properties, "x", pos.x);
        xml::getAttribute(properties, "y", pos.y);
        xml::getAttribute(properties, "z", pos.z);
    }
    transform.position = pos;

    // quat rot = {1, 0, 0, 0};
    // if(xml::getAttribute(properties, "rotation", rot)) {
    //     transform = transform * glm::mat4_cast(rot);
    // }
    return true;
}

/// This Component stores the 3D position, roation and scale of an entity
export using Transform = BasicComponent<"Transform", TransformationData>;

} // namespace bembel::kernel
