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

export enum class VertexAttrib : uint {
    POSITION,
    NORMAL,
    TANGENT,
    TEX_COORDS,
    BONE_INDICES,
    BONE_WEIGHTS
};

export enum class VertexAttribMask : u64 {
    POSITION     = 1 << u64(VertexAttrib::POSITION),
    NORMAL       = 1 << u64(VertexAttrib::NORMAL),
    TANGENT      = 1 << u64(VertexAttrib::TANGENT),
    TEX_COORDS   = 1 << u64(VertexAttrib::TEX_COORDS),
    BONE_INDICES = 1 << u64(VertexAttrib::BONE_INDICES),
    BONE_WEIGHTS = 1 << u64(VertexAttrib::BONE_WEIGHTS)
};

export VertexAttribMask operator|(In<VertexAttribMask> a, In<VertexAttribMask> b) {
    return VertexAttribMask(std::to_underlying(a) | std::to_underlying(b));
}
export VertexAttribMask operator&(In<VertexAttribMask> a, In<VertexAttribMask> b) {
    return VertexAttribMask(std::to_underlying(a) & std::to_underlying(b));
}

export VertexAttribMask& operator|=(InOut<VertexAttribMask> a, In<VertexAttribMask> b) {
    a = VertexAttribMask(std::to_underlying(a) | std::to_underlying(b));
    return a;
}

export struct DefaultVertexFormat {
    DefaultVertexFormat(In<vec3> pos, In<vec3> n, In<vec3> t, In<vec2> tx)
      : position{pos}
      , normal{0x7FFF * n.x, 0x7FFF * n.y, 0x7FFF * n.z}
      , tangent{0x7FFF * t.x, 0x7FFF * t.y, 0x7FFF * t.z}
      , tex_coords{tx} {}
    // OpenGL 4.2+ u16 to normalized float:
    //     float = max(int/0x7FFF, -1.0f)
    // <=> int   = 0x7FFF*float

    vec3            position;   //  12 Byte ->  12 Byte
    glm::tvec3<i16> normal;     // + 6 Byte ->  18 Byte
    glm::tvec3<i16> tangent;    // + 6 Byte ->  24 Byte
    vec2            tex_coords; // + 8 Byte ->  32 Byte
};
static_assert(sizeof(DefaultVertexFormat) == 32);

export struct RiggedVertexFormat {
    RiggedVertexFormat(In<vec3> pos, In<quat> n, In<vec3> t, In<vec2> tx, In<ivec4> bi, In<vec4> bw)
      : position{pos}
      , normal{0x7FFF * n.x, 0x7FFF * n.y, 0x7FFF * n.z}
      , tangent{0x7FFF * t.x, 0x7FFF * t.y, 0x7FFF * t.z}
      , tex_coords{tx}
      , bone_indices{bi}
      , bone_weights{bw} {} // OpenGL 4.2+ u16 to normalized float:
                            //     float = max(int/0x7FFF, -1.0f)
                            // <=> int   = 0x7FFF*float

    vec3            position;     //  12 Byte ->  12 Byte
    glm::tvec3<i16> normal;       // + 6 Byte ->  18 Byte
    glm::tvec3<i16> tangent;      // + 6 Byte ->  24 Byte
    vec2            tex_coords;   // +  8 Byte -> 32 Byte
    ivec4           bone_indices; // + 16 Byte -> 48 Byte
    vec4            bone_weights; // + 16 Byte -> 64 Byte
};
static_assert(sizeof(RiggedVertexFormat) == 64);

export class GeometryMesh {
  public:
    struct SubMesh {
        uint first_index;
        uint num_indices;
    };

  public:
    GeometryMesh(VertexAttribMask format);
    GeometryMesh(In<std::span<DefaultVertexFormat>>, In<std::span<uint>>);
    GeometryMesh(In<std::span<RiggedVertexFormat>>, In<std::span<uint>>);
    GeometryMesh(GeometryMesh const&) = delete;
    GeometryMesh(GeometryMesh&&)      = delete;
    ~GeometryMesh();

    VertexAttribMask       getVertexFormat() const { return m_vertex_format; }
    std::optional<uint>    getSubMeshIndex(std::string_view name);
    std::optional<SubMesh> getSubMesh(std::string_view name);
    std::optional<SubMesh> getSubMesh(uint index);
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

    VertexAttribMask m_vertex_format;

    std::vector<std::pair<std::string, SubMesh>> m_sub_meshs;
};

} // namespace bembel::graphics
