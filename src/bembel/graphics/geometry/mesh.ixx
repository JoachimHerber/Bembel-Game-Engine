module;
#include <filesystem>
#include <memory>
#include <optional>
#include <string_view>
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

    struct SubMesh {
        uint first_index;
        uint num_indices;
    };
    std::optional<SubMesh> getSubMesh(std::string_view name);
    uint                   getVAO() const;

    static std::unique_ptr<GeometryMesh> loadAsset(
         In<std::filesystem::path> file
    );
    static std::unique_ptr<GeometryMesh> createAsset(
         xml::Element const* properties
    );

    static constexpr std::string_view ASSET_TYPE_NAME = "GeometryMesh";

    using DefaultLoaderType = SerialAssetLoader<GeometryMesh>;

  private:
    static std::unique_ptr<GeometryMesh> createGeometryMesh(xml::Element const* properties);

    static bool parseVertexData(xml::Element const*, std::vector<float>&);
    static bool parseIndexData(xml::Element const*, std::vector<unsigned>&);

  private:
    uint m_vao = 0;
    uint m_vbo = 0;
    uint m_ibo = 0;

    Dictionary<SubMesh> m_sub_meshs;
};

} // namespace bembel::graphics
