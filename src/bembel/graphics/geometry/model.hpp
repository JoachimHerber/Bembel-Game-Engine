#ifndef BEMBEL_GRAPHICS_GEOMETRY_MODEL_HPP
#define BEMBEL_GRAPHICS_GEOMETRY_MODEL_HPP

#include <bembel/kernel/assets/asset-container.hpp>
#include <bembel/kernel/assets/asset-handle.hpp>
#include <bembel/kernel/assets/serial-asset-loader.hpp>
#include <bembel/kernel/open-gl.hpp>

#include "./material.hpp"
#include "./mesh.hpp"

namespace bembel::graphics {

class BEMBEL_API GeometryModel {
  public:
    using DefaultLoaderType = kernel::SerialAssetLoader<GeometryModel>;

  public:
    GeometryModel()  = default;
    ~GeometryModel() = default;

    struct MaterialMapping {
        kernel::Asset<Material> material;
        std::string             sub_mesh;
    };

    GeometryMesh*                       getMesh();
    const std::vector<MaterialMapping>& getMateialMapping();

    const static std::string& getTypeName();

    static std::unique_ptr<GeometryModel> loadAsset(
        kernel::AssetManager& asset_mgr, const std::string& file_name);
    static std::unique_ptr<GeometryModel> createAsset(
        kernel::AssetManager& asset_mgr, const base::xml::Element* properties);

    static void deleteAsset(kernel::AssetManager& asset_mgr, std::unique_ptr<GeometryModel> model);

  private:
    static std::unique_ptr<GeometryModel> createGeometryModel(
        kernel::AssetManager& asset_mgr, const base::xml::Element* properties);

  private:
    kernel::Asset<GeometryMesh>  mesh;
    std::vector<MaterialMapping> material_mapping;
};

using GeometryModelContainer    = kernel::AssetContainer<GeometryModel>;
using GeometryModelContainerPtr = std::shared_ptr<GeometryModelContainer>;

} // namespace bembel::graphics
#endif // include guards
