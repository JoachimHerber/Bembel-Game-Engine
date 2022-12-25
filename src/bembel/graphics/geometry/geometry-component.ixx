module;
export module bembel.graphics.geometry:Components;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export struct GeometryComponentData {
    using RendererID = uint;

    AssetHandle model;
    vec3        scale       = {1, 1, 1};
    RendererID  renderer    = 0;
    bool        cast_shadow = true;
};

export bool initComponent(
    In<xml::Element const*>      properties,
    InOut<AssetManager>          asset_mgr,
    InOut<GeometryComponentData> component
);

export using Geometry = BasicComponent<"Geometry", GeometryComponentData>;

} // namespace bembel::graphics
