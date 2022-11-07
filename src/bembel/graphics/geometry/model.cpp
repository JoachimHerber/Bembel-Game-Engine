module;
#include "bembel/pch.h"
module bembel.graphics.geometry;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

GeometryMesh* GeometryModel::getMesh() {
    return m_mesh.getAsset();
}

std::vector<GeometryModel::MaterialMapping> const& GeometryModel::getMateialMapping() {
    return m_material_mapping;
}

std::unique_ptr<GeometryModel> GeometryModel::loadAsset(
    AssetManager& asset_mgr, std::filesystem::path file
) {
    std::string const file_path = file.string(); // file.c_str() returns a wchar*
    xml::Document     doc;
    if(doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        log().error("Failed to load file '{}' \n {}", file_path, doc.ErrorName());
        return nullptr;
    }

    xml::Element const* root = doc.FirstChildElement("GeometryModel");
    if(!root) {
        log().error("File '{}' has no root element 'GeometryModel'", file_path);
        return nullptr;
    }
    return GeometryModel::createGeometryModel(asset_mgr, root);
}

std::unique_ptr<GeometryModel> GeometryModel::createAsset(
    AssetManager& asset_mgr, xml::Element const* properties
) {
    return GeometryModel::createGeometryModel(asset_mgr, properties);
}

void GeometryModel::deleteAsset(AssetManager&, std::unique_ptr<GeometryModel>) {}

std::unique_ptr<GeometryModel> GeometryModel::createGeometryModel(
    AssetManager& asset_mgr, xml::Element const* properties
) {
    auto model = std::make_unique<GeometryModel>();

    std::string mesh;
    xml::getAttribute(properties, "mesh", mesh);
    model->m_mesh.request(asset_mgr, mesh);

    for(xml::Element const* it : xml::IterateChildElements(properties, "MaterialMapping")) {
        MaterialMapping mapping;
        std::string     material;
        if(xml::getAttribute(it, "material", material)
           && xml::getAttribute(it, "submesh", mapping.sub_mesh)
           && mapping.material.request(asset_mgr, material)) {
            model->m_material_mapping.push_back(std::move(mapping));
        }
    }

    return std::move(model);
}

} // namespace bembel::graphics
