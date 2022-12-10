module;
#include "bembel/pch.h"
module bembel.graphics.geometry;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

bool initComponent(
    In<xml::Element const*>      properties,
    InOut<AssetManager>          asset_mgr,
    InOut<GeometryComponentData> geometry_component
) {
    std::string model_name;
    if(!xml::getAttribute(properties, "model", model_name)) return false;

    geometry_component.model = asset_mgr.getAssetHandle<GeometryModel>(model_name);

    return true;
}

} // namespace bembel::graphics
