export module bembel.graphics.pipeline:DeferredLightingStage;

import bembel.base;
import bembel.kernel;
import bembel.graphics.geometry;
import :RenderingPipeline;
import :LightSource;
import :ShadowMap;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export class DeferredLightingStage : public RenderingPipeline::Stage {
  public:
    DeferredLightingStage(RenderingPipeline& pipline);
    ~DeferredLightingStage();

    void setDirLightShader(Asset<ShaderProgram>);
    void setPointLightShader(Asset<ShaderProgram>);

    void setDirLightShadowResolution(uint resolution);
    template <typename... TArgs>
        requires(std::is_convertible_v<TArgs, float>&&...)
    void setDirLightShadowCascadeDistances(TArgs&& ...cascades) {
        m_dir_lights.cascade_distances.clear();
        (m_dir_lights.cascade_distances.push_back(cascades),...);
    }
    void setDirLightShadowCascadeDistances(In<std::span<const float>> cascades);

    virtual bool configure(xml::Element const*) override;

    virtual void setScene(Scene*) override;

    virtual void init() override;
    virtual void cleanup() override;
    virtual void execute(In<std::span<const RendererPtr>> renderer) override;

    ShadowMap& getDirectionalLightShadowMap() { return m_dir_lights.shadows; }

    using RenderingPipeline::Stage::setColorOutputTexture;
    using RenderingPipeline::Stage::setInputTextures;

  private:
    void updateShadowMaps(
        In<std::span<const RendererPtr>>         renderer,
        In<std::span<const std::array<vec4, 8>>> frustum_corners
    );
    void initRenderQueueForShadowPass();
    void applyDirectionalLights(In<std::span<const RendererPtr>> renderer);
    void applyPointLights(In<std::span<const RendererPtr>> renderer);

    void beginRenderToTexture();
    void endRenderToTexture();

  private:
    struct PointLights {
        Asset<ShaderProgram> shader;
        uint                 vao         = 0;
        uint                 vbo         = 0;
        uint                 buffer_size = 0;
    };

    struct DirectionalLights {
        Asset<ShaderProgram> shader;
        ShadowMap            shadows           = {Texture::Target::TEXTURE_2D_ARRAY};
        std::vector<float>   cascade_distances = {0.01f, 5.0f, 15.0f, 100.0f};
    };

    PointLights       m_point_lights;
    DirectionalLights m_dir_lights;
    Scene*            m_scene = nullptr;

    GeometryRenderQueue m_render_queue;
};

} // namespace bembel::graphics
