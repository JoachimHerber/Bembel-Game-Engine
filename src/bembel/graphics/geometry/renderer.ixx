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

    GeometryRendererBase(AssetManager& asset_mgr, RendererId id)
      : m_asset_mgr{asset_mgr}, m_id{id} {};
    virtual ~GeometryRendererBase(){};

    RendererId getRendererID() const { return m_id; };

    virtual void render(
        mat4 const& proj, mat4 const& view, std::vector<GeometryRenderData> const& data
    ) = 0;

    virtual std::unique_ptr<Material> createMaterial(xml::Element const* propertiey) = 0;

  protected:
    AssetManager&    m_asset_mgr;
    const RendererId m_id;
};

export class DefaultGeometryRenderer : public GeometryRendererBase {
  public:
    DefaultGeometryRenderer(AssetManager& asset_mgr, uint id)
      : GeometryRendererBase{asset_mgr, id} {}
    ~DefaultGeometryRenderer() = default;

    void addRequiredTexture(std::string_view texture_name, std::string_view uniform_sampler_name) {
        m_required_textures.emplace_back(texture_name, uniform_sampler_name);
    }

    bool setShader(Asset<ShaderProgram> shader) {
        if(updateUniformLocations(shader.getAsset())) {
            m_shader = std::move(shader);
            return true;
        }
        return false;
    }

    virtual void render(
        mat4 const& proj, mat4 const& view, std::vector<GeometryRenderData> const& data
    ) override;

    virtual std::unique_ptr<Material> createMaterial(xml::Element const* propertiey) override;

    static std::unique_ptr<DefaultGeometryRenderer> createRenderer(
        xml::Element const*, AssetManager&, unsigned id
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
    Asset<ShaderProgram> m_shader;

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
