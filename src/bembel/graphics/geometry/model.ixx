export module bembel.graphics.geometry:Model;

import bembel.base;
import bembel.kernel;
import :Material;
import :Mesh;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export class GeometryModel {
  public:
  public:
    GeometryModel()  = default;
    ~GeometryModel() = default;

    struct MaterialMapping {
        Asset<Material> material;
        std::string     sub_mesh;
    };

    GeometryMesh*                       getMesh();
    std::vector<MaterialMapping> const& getMateialMapping();

    static constexpr std::string_view ASSET_TYPE_NAME = "GeometryModel";

    static std::unique_ptr<GeometryModel> loadAsset(
        AssetManager& asset_mgr, std::filesystem::path file
    );
    static std::unique_ptr<GeometryModel> createAsset(
        AssetManager& asset_mgr, xml::Element const* properties
    );

    static void deleteAsset(AssetManager& asset_mgr, std::unique_ptr<GeometryModel> model);

    using DefaultLoaderType = SerialAssetLoader<GeometryModel>;

  private:
    static std::unique_ptr<GeometryModel> createGeometryModel(
        AssetManager& asset_mgr, xml::Element const* properties
    );

  private:
    Asset<GeometryMesh>          m_mesh;
    std::vector<MaterialMapping> m_material_mapping;
};

} // namespace bembel::graphics
