module;
#include <glbinding/gl/gl.h>

#include <utility>
export module bembel.graphics.geometry:Renderer;

import bembel.base;
import bembel.kernel;
import :Material;
import :Mesh;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export struct GeometryRenderData {
    GeometryMesh*         mesh;
    Material*             material;
    GeometryMesh::SubMesh sub_mesh;
    mat4                  transform;
};

export class GeometryRendererBase {
  public:
    using VertexFormat = GeometryMesh::VertexFormat;

    GeometryRendererBase(VertexFormat vertex_format) : m_vertex_format{vertex_format} {};
    virtual ~GeometryRendererBase(){};

    VertexFormat getVertexFormat() const { return m_vertex_format; }

    virtual void renderGeometry(
        In<mat4> proj, In<mat4> view, In<std::vector<GeometryRenderData>> data
    ) = 0;

    virtual void renderShadows(In<mat4> view_proj, In<std::vector<GeometryRenderData>> data) = 0;

  protected:
    VertexFormat m_vertex_format;
};

export class DefaultGeometryRenderer : public GeometryRendererBase {
  public:
    DefaultGeometryRenderer(VertexFormat vertex_format) : GeometryRendererBase{vertex_format} {}
    ~DefaultGeometryRenderer() = default;

    bool setShaders(In<Asset<ShaderProgram>> geom_pass, In<Asset<ShaderProgram>> depth_pass) {
        if(Material::verifyUniformBlockLayout(geom_pass.get())) {
            m_geomety_pass_shader = std::move(geom_pass);
            m_depth_pass_shader   = std::move(depth_pass);
            return true;
        }
        return false;
    }

    virtual void renderGeometry(
        In<mat4> proj, In<mat4> view, In<std::vector<GeometryRenderData>> data
    ) override;

    virtual void renderShadows(In<mat4> view_proj, In<std::vector<GeometryRenderData>> data)
        override;

    static std::unique_ptr<DefaultGeometryRenderer> createRenderer(
        xml::Element const*, VertexFormat vertex_format
    );

  private:
    void initDummyTextures();

  private:
    Asset<ShaderProgram> m_geomety_pass_shader;
    Asset<ShaderProgram> m_depth_pass_shader;

    uint m_material_uniform_block_index;

    std::unique_ptr<Texture> m_dummy_emission_texture;
    std::unique_ptr<Texture> m_dummy_base_color_texture;
    std::unique_ptr<Texture> m_dummy_material_props_texture;
    std::unique_ptr<Texture> m_dummy_normal_map_texture;
};

} // namespace bembel::graphics
