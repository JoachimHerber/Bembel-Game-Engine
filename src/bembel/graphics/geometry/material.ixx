module;
#include <string_view>
#include <vector>
export module bembel.graphics.geometry:Material;

import bembel.base;
import bembel.kernel;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export class Material final {
  public:
    Material();
    Material(Material const&) = delete;
    Material(Material&&)      = delete;
    ~Material();

    void setEmission(In<vec3> emission);
    void setBaseColor(In<vec3> base_color);
    void setRoughness(In<float> roughness);
    void setMetallic(In<float> metallic);
    void setF0(In<float> f0);
    void setEmissionTexture(Asset<Texture> texture) { m_textures.emission = texture; }
    void setBaseColorTexture(Asset<Texture> texture) { m_textures.base_color = texture; }
    void setMaterialPropsTexture(Asset<Texture> texture) { m_textures.material_props = texture; }
    void setNormalMapTexture(Asset<Texture> texture) { m_textures.normal_map = texture; }

    void bindUniformBufferObject();

    vec3                  getEmission() const { return m_params.emission; }
    vec3                  getBaseColor() const { return m_params.base_color; }
    float                 getRoughness() const { return m_params.roughness; }
    float                 getMetallic() const { return m_params.metallic; }
    float                 getF0() const { return m_params.f0; }
    Asset<Texture> const& getEmissionTexture() const { return m_textures.emission; }
    Asset<Texture> const& getBaseColorTexture() const { return m_textures.base_color; }
    Asset<Texture> const& getMaterialPropsTexture() const { return m_textures.material_props; }
    Asset<Texture> const& getNormalMapTexture() const { return m_textures.normal_map; }

    static std::unique_ptr<Material> createAsset(xml::Element const* properties);

    static constexpr std::string_view ASSET_TYPE_NAME = "Material";

    using DefaultLoaderType = SerialAssetLoader<Material>;

  public:
    static bool verifyUniformBlockLayout(ShaderProgram* shader);

  private:
    struct Textures {
        Asset<Texture> emission;
        Asset<Texture> base_color;
        Asset<Texture> material_props; // RGB = (roughness, metallic, f0)
        Asset<Texture> normal_map;
    };

    struct Params {
        //                                          base alignment | aligned offset
        vec4  emission   = {0.f, 0.f, 0.f, 1.f}; //            16  |   0
        vec4  base_color = {1.f, 1.f, 1.f, 1.f}; //            16  |  16
        float roughness  = 1.0;                  //             4  |  32
        float metallic   = 1.0;                  //             4  |  36
        float f0         = 1.0;                  //             4  |  40
        float unused     = 1.0;                  //             4  |  44
    };

    uint     m_uniform_buffer_object = 0;
    Textures m_textures;
    Params   m_params;
    bool     m_params_dirty;
};

} // namespace bembel::graphics