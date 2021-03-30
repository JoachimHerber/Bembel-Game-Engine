#ifndef BEMBEL_GRAPHICS_GEOMETRY_RENDERER_HPP
#define BEMBEL_GRAPHICS_GEOMETRY_RENDERER_HPP

#include <bembel/base/io/xml.hpp>
#include <bembel/kernel/assets/asset.hpp>
#include <bembel/kernel/open-gl.hpp>
#include <bembel/kernel/rendering/shader.hpp>

namespace bembel::graphics {

class GeometryMesh;
class Material;

struct GeometryRenderData {
    GeometryMesh* mesh;
    Material*     material;
    unsigned      first;
    unsigned      count;
    glm::mat4     transform;
};

class BEMBEL_API GeometryRendererBase {
  public:
    GeometryRendererBase(kernel::AssetManager& asset_mgr, unsigned id)
    : asset_mgr{asset_mgr}
    , id{id} {};
    virtual ~GeometryRendererBase(){};

    unsigned getRendererID() const { return id; };

    virtual void render(
        const glm::mat4&                       proj,
        const glm::mat4&                       view,
        const std::vector<GeometryRenderData>& data) = 0;

    virtual std::unique_ptr<Material> createMaterial(const base::xml::Element* propertiey) = 0;

  protected:
    kernel::AssetManager& asset_mgr;
    const unsigned        id;
};

class BEMBEL_API DefaultGeometryRenderer : public GeometryRendererBase {
  public:
    DefaultGeometryRenderer(kernel::AssetManager& asset_mgr, unsigned id);
    ~DefaultGeometryRenderer();

    void addRequiredTexture(
        const std::string& texture_name, const std::string& uniform_sampler_name);

    bool setShader(const kernel::Asset<kernel::ShaderProgram>& shader);

    virtual void render(
        const glm::mat4&                       proj,
        const glm::mat4&                       view,
        const std::vector<GeometryRenderData>& data) override;

    virtual std::unique_ptr<Material> createMaterial(const base::xml::Element* propertiey) override;

    static std::unique_ptr<DefaultGeometryRenderer> createRenderer(
        const base::xml::Element*, kernel::AssetManager&, unsigned id);

  private:
    struct MaterialParam {
        GLint  offset;
        GLint  size;
        GLenum type;
    };

    bool updateUniformLocations(kernel::ShaderProgram*);

    bool readMaterialParameter(
        const base::xml::Element* properties,
        const std::string&        param_name,
        const MaterialParam&      param,
        GLbyte*                   buffer);

  private:
    kernel::Asset<kernel::ShaderProgram> shader;

    GLuint material_uniform_block_index;
    GLuint material_uniform_buffer_size;

    std::map<std::string, MaterialParam> material_params;

    struct RequiredTexture {
        std::string texture_name;
        std::string uniform_sampler_name;
    };
    std::vector<RequiredTexture> required_textures;
};

} // namespace bembel::graphics
#endif // include guards
