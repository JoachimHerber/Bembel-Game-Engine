#include "./geometry.hpp"

#include <bembel/kernel/assets/asset-manager.hpp>

#include "./model.hpp"

namespace bembel::graphics {

const std::string& GeometryComponent::getComponentTypeName() {
    const static std::string type_name = "GeometryComponent";
    return type_name;
}

bool GeometryComponent::initComponent(
    kernel::AssetManager&     asset_mgr,
    const base::xml::Element* properties,
    GeometryComponent*        geometry_component) {
    std::string model_name;
    if(!base::xml::getAttribute(properties, "model", model_name)) return false;

    geometry_component->model = asset_mgr.getAssetHandle<GeometryModel>(model_name);

    return true;
}

} // namespace bembel::graphics
