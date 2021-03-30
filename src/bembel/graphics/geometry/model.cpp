#include "./model.hpp"

#include "./mesh.hpp"

namespace bembel::graphics {

GeometryMesh* GeometryModel::getMesh() {
    return this->mesh.get();
}

const std::vector<GeometryModel::MaterialMapping>& GeometryModel::getMateialMapping() {
    return this->material_mapping;
}

const std::string& GeometryModel::getTypeName() {
    const static std::string type_name = "GeometryModel";
    return type_name;
}

std::unique_ptr<GeometryModel> GeometryModel::loadAsset(
    kernel::AssetManager& asset_mgr, const std::string& file_name) {
    base::xml::Document doc;
    if(doc.LoadFile(file_name.c_str()) != tinyxml2::XML_SUCCESS) {
        BEMBEL_LOG_ERROR() << "Failed to load file '" << file_name << "'\n"
                           << doc.ErrorName() << std::endl;
        return nullptr;
    }

    const base::xml::Element* root = doc.FirstChildElement("GeometryModel");
    if(!root) {
        BEMBEL_LOG_ERROR() << "File '" << file_name << "' has no root element 'GeometryModel'"
                           << std::endl;
        return nullptr;
    }
    return GeometryModel::createGeometryModel(asset_mgr, root);
}

std::unique_ptr<GeometryModel> GeometryModel::createAsset(
    kernel::AssetManager& asset_mgr, const base::xml::Element* properties) {
    return GeometryModel::createGeometryModel(asset_mgr, properties);
}

void GeometryModel::deleteAsset(kernel::AssetManager&, std::unique_ptr<GeometryModel>) {
}

std::unique_ptr<GeometryModel> GeometryModel::createGeometryModel(
    kernel::AssetManager& asset_mgr, const base::xml::Element* properties) {
    auto model = std::make_unique<GeometryModel>();

    std::string mesh;
    base::xml::getAttribute(properties, "mesh", mesh);
    model->mesh.request(asset_mgr, mesh);

    for(const base::xml::Element* it :
        base::xml::IterateChildElements(properties, "MaterialMapping")) {
        MaterialMapping mapping;
        std::string     material;
        if(base::xml::getAttribute(it, "material", material)
           && base::xml::getAttribute(it, "submesh", mapping.sub_mesh)
           && mapping.material.request(asset_mgr, material)) {
            model->material_mapping.push_back(std::move(mapping));
        }
    }

    return std::move(model);
}

} // namespace bembel::graphics
