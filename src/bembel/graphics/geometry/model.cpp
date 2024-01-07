module;
#include <filesystem>
module bembel.graphics.geometry;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

GeometryModel::GeometryModel(In<Asset<GeometryMesh>> mesh) : m_mesh{mesh} {}

GeometryMesh* GeometryModel::getMesh() {
    return m_mesh.get();
}

std::vector<GeometryModel::MaterialMapping> const& GeometryModel::getMateialMapping() {
    return m_material_mapping;
}

void GeometryModel::setMaterialMapping(In<std::string_view> sub_mesh, In<Asset<Material>> mat) {
    for(auto& it : m_material_mapping) {
        if(it.sub_mesh == sub_mesh) {
            it.material = mat;
            return;
        }
    }
    MaterialMapping mm;
    mm.material = mat;
    mm.sub_mesh = sub_mesh;
    m_material_mapping.push_back(std::move(mm));
}

std::unique_ptr<GeometryModel> GeometryModel::loadAsset(std::filesystem::path file) {
    std::string const file_path = file.string(); // file.c_str() returns a wchar*
    xml::Document     doc;
    if(doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        logError("Failed to load file '{}' \n {}", file_path, doc.ErrorName());
        return nullptr;
    }

    xml::Element const* root = doc.FirstChildElement("GeometryModel");
    if(!root) {
        logError("File '{}' has no root element 'GeometryModel'", file_path);
        return nullptr;
    }
    return GeometryModel::createGeometryModel(root);
}

std::unique_ptr<GeometryModel> GeometryModel::createAsset(xml::Element const* properties) {
    return GeometryModel::createGeometryModel(properties);
}

std::unique_ptr<GeometryModel> GeometryModel::createGeometryModel(xml::Element const* properties) {
    auto model = std::make_unique<GeometryModel>();

    std::string mesh;
    xml::getAttribute(properties, "mesh", mesh);
    model->m_mesh.request(mesh);

    for(xml::Element const* it : xml::IterateChildElements(properties, "MaterialMapping")) {
        MaterialMapping mapping;
        std::string     material;
        if(xml::getAttribute(it, "material", material)
           && xml::getAttribute(it, "submesh", mapping.sub_mesh)
           && mapping.material.request(material)) {
            model->m_material_mapping.push_back(std::move(mapping));
        }
    }

    return std::move(model);
}

} // namespace bembel::graphics
