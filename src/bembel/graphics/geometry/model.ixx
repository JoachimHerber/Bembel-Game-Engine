module;
#include <string>
#include <vector>
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
    struct MaterialMapping {
        Asset<Material> material;
        std::string     sub_mesh;
    };

  public:
    GeometryModel() = default;
    GeometryModel(In<Asset<GeometryMesh>>);
    ~GeometryModel() = default;

    GeometryMesh*                       getMesh();
    std::vector<MaterialMapping> const& getMateialMapping();

    void setMaterialMapping(In<std::string_view> sub_mesh, In<Asset<Material>> mat);

    static constexpr std::string_view ASSET_TYPE_NAME = "GeometryModel";

    static std::unique_ptr<GeometryModel> loadAsset(std::filesystem::path file);
    static std::unique_ptr<GeometryModel> createAsset(xml::Element const* properties);

    using DefaultLoaderType = SerialAssetLoader<GeometryModel>;

  private:
    static std::unique_ptr<GeometryModel> createGeometryModel(xml::Element const* properties);

  private:
    Asset<GeometryMesh>          m_mesh;
    std::vector<MaterialMapping> m_material_mapping;
};

} // namespace bembel::graphics
