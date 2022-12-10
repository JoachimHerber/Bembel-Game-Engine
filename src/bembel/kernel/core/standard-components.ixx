module;
#include "bembel/pch.h"
export module bembel.kernel.core:StandardComponents;

import bembel.base;
import bembel.kernel.assets;

import :Components;

namespace bembel::kernel {
using namespace bembel::base;

export bool initComponent(xml::Element const* properties, AssetManager& asset_mgr, vec3& pos) {
    return xml::getAttribute(properties, "x", pos.x) //
        && xml::getAttribute(properties, "y", pos.y) //
        && xml::getAttribute(properties, "z", pos.z);
}

/// This Component stores the 3D position of an entity
export using PositionComponent = StandardComponent<"Position", vec3>;

export bool initComponent(xml::Element const* properties, AssetManager& asset_mgr, quat& rot) {
    return xml::getAttribute(properties, "x", rot.x) && xml::getAttribute(properties, "y", rot.y)
        && xml::getAttribute(properties, "z", rot.z) && xml::getAttribute(properties, "w", rot.w);
}

/// This Component stores the 3D rotation of an entity
export using RotationComponent = StandardComponent<"Rotation", quat>;

} // namespace bembel::kernel
