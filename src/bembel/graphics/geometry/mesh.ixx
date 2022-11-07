module;
#include "bembel/pch.h"
export module bembel.graphics.geometry:Mesh;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export class GeometryMesh {
  public:
    GeometryMesh();
    GeometryMesh(GeometryMesh const&) = delete;
    GeometryMesh(GeometryMesh&&)      = delete;
    ~GeometryMesh();

    using GeometryMeshPtr = std::unique_ptr<GeometryMesh>;

    bool getSubMesh(std::string_view name, uint& first_index, uint& num_indices);
    uint getVAO() const;

    static GeometryMeshPtr loadAsset(AssetManager& asset_mgr, std::filesystem::path file);
    static GeometryMeshPtr createAsset(AssetManager& asset_mgr, xml::Element const* properties);

    static void deleteAsset(AssetManager& asset_mgr, std::unique_ptr<GeometryMesh> mesh);

    static constexpr std::string_view ASSET_TYPE_NAME = "GeometryMesh";

    using DefaultLoaderType = SerialAssetLoader<GeometryMesh>;

  private:
    static GeometryMeshPtr createGeometryMesh(xml::Element const* properties);

    static bool parseVertexData(xml::Element const*, std::vector<float>&);
    static bool parseIndexData(xml::Element const*, std::vector<unsigned>&);

  private:
    uint m_vao = 0;
    uint m_vbo = 0;
    uint m_ibo = 0;

    struct SubMesh {
        uint first_index;
        uint num_indices;
    };
    Dictionary<SubMesh> m_sub_meshs;
};

} // namespace bembel::graphics
