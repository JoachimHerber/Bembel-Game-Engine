module;
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <optional>
#include <span>
#include <string_view>
export module bembel.graphics.geometry:Mesh;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;


export struct DefaultVertexFormat {
    DefaultVertexFormat(
        In<vec3> pos, In<vec3> n, In<vec3> t,  In<vec2> tx)
      : position{pos}
      , normal{0x7FFF * n.x, 0x7FFF * n.y, 0x7FFF * n.z}
      , tangent{0x7FFF * t.x, 0x7FFF * t.y, 0x7FFF * t.z}
      // OpenGL 4.2+ u16 to normalized float:
      //     float = max(int/0x7FFF, -1.0f)
      // <=> int   = 0x7FFF*float
      , tex_coords{tx} {}
    vec3            position;   //  12 Byte ->  12 Byte
    glm::tvec3<i16> normal;     // + 6 Byte ->  18 Byte
    glm::tvec3<i16> tangent;    // + 6 Byte ->  24 Byte
    vec2            tex_coords; // + 8 Byte ->  32 Byte
};
static_assert(sizeof(DefaultVertexFormat) == 32);

export struct RiggedVertexFormat {
    RiggedVertexFormat(In<vec3> pos, In<quat> n, In<vec2> tx, In<ivec4> bi, In<vec4> bw)
      : position{pos, 1.0f}
      , normal{0x7FFF * n.x, 0x7FFF * n.y, 0x7FFF * n.z, 0x7FFF * n.w}
      // OpenGL 4.2+ u16 to normalized float:
      //     float = max(int/0x7FFF, -1.0f)
      // <=> int   = 0x7FFF*float
      , tex_coords{tx}
      , bone_indices{bi}
      , bone_weights{bw} {}

    vec4            position;     //   12 Byte -> 12 Byte
    glm::tvec4<i16> normal;       // +  8 Byte -> 24 Byte (quaternion)
    vec2            tex_coords;   // +  8 Byte -> 32 Byte
    ivec4           bone_indices; // + 16 Byte -> 48 Byte
    vec4            bone_weights; // + 16 Byte -> 64 Byte
};
static_assert(sizeof(RiggedVertexFormat) == 64);

export class GeometryMesh {
  public:
    enum class VertexFormat { Default, Rigged };

    struct SubMesh {
        uint first_index;
        uint num_indices;
    };

  public:
    GeometryMesh(VertexFormat format);
    GeometryMesh(In<std::span<DefaultVertexFormat>>, In<std::span<uint>>);
    GeometryMesh(In<std::span<RiggedVertexFormat>>, In<std::span<uint>>);
    GeometryMesh(GeometryMesh const&) = delete;
    GeometryMesh(GeometryMesh&&)      = delete;
    ~GeometryMesh();

    VertexFormat           getVertexFormat() const { return m_vertex_format; }
    std::optional<SubMesh> getSubMesh(std::string_view name);
    uint                   getVAO() const;

    void setSubMesh(std::string_view name, uint first_index, uint num_indices);

    static std::unique_ptr<GeometryMesh> loadAsset(In<std::filesystem::path> file);
    static std::unique_ptr<GeometryMesh> createAsset(xml::Element const* properties);

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

    VertexFormat m_vertex_format = VertexFormat::Default;

    Dictionary<SubMesh> m_sub_meshs;
};

} // namespace bembel::graphics
