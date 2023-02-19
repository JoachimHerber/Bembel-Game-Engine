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
    using RendererId = uint;

    GeometryRendererBase( RendererId id)
      :  m_id{id} {};
    virtual ~GeometryRendererBase(){};

    RendererId getRendererID() const { return m_id; };

    virtual void renderGeometry(
        In<mat4> proj, In<mat4> view, In<std::vector<GeometryRenderData>> data
    ) = 0;

    virtual void renderShadows(
        In<mat4> view_proj, In<std::vector<GeometryRenderData>> data
    ) = 0;

    virtual std::unique_ptr<Material> createMaterial(xml::Element const* propertiey) = 0;

  protected:
    const RendererId m_id;
};

export class DefaultGeometryRenderer : public GeometryRendererBase {
  public:
    DefaultGeometryRenderer(uint id)
      : GeometryRendererBase{id} {}
    ~DefaultGeometryRenderer() = default;

    void addRequiredTexture(std::string_view texture_name, std::string_view uniform_sampler_name) {
        m_required_textures.emplace_back(texture_name, uniform_sampler_name);
    }

    bool setShaders(Asset<ShaderProgram> geom_pass, Asset<ShaderProgram> depth_pass) {
        if(updateUniformLocations(geom_pass.get())) {
            m_geomety_pass_shader = std::move(geom_pass);
            m_depth_pass_shader = std::move(depth_pass);
            return true;
        }
        return false;
    }

    virtual void renderGeometry(
        In<mat4> proj, In<mat4> view, In<std::vector<GeometryRenderData>> data
    ) override;

    virtual void renderShadows(
        In<mat4> view_proj, In<std::vector<GeometryRenderData>> data
    ) override;

    virtual std::unique_ptr<Material> createMaterial(xml::Element const* propertiey) override;

    static std::unique_ptr<DefaultGeometryRenderer> createRenderer(
        xml::Element const*, unsigned id
    );

  private:
    struct MaterialParam {
        int        offset;
        int        size;
        gl::GLenum type;
    };

    bool updateUniformLocations(ShaderProgram*);

    bool readMaterialParameter(
        xml::Element const*  properties,
        std::string_view     param_name,
        MaterialParam const& param,
        byte*                buffer
    );

  private:
    Asset<ShaderProgram> m_geomety_pass_shader;
    Asset<ShaderProgram> m_depth_pass_shader;

    uint m_material_uniform_block_index;
    uint m_material_uniform_buffer_size;

    Dictionary<MaterialParam> m_material_params;

    struct RequiredTexture {
        std::string texture_name;
        std::string uniform_sampler_name;

        RequiredTexture(std::string_view texture, std::string_view sampler)
          : texture_name(texture), uniform_sampler_name(sampler) {}
    };
    std::vector<RequiredTexture> m_required_textures;
};

} // namespace bembel::graphics
