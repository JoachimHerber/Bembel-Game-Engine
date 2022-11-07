export module bembel.graphics.pipeline:DeferredLightingStage;

import bembel.base;
import bembel.kernel;
import bembel.graphics.geometry;
import :RenderingPipeline;
import :LightSource;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export class DeferredLightingStage : public RenderingPipeline::Stage {
  public:
    DeferredLightingStage(RenderingPipeline& pipline);
    ~DeferredLightingStage();

    void setDirLightShader(Asset<ShaderProgram>);
    void setPointLightShader(Asset<ShaderProgram>);

    virtual bool configure(xml::Element const*) override;

    virtual void setScene(Scene*) override;

    virtual void init() override;
    virtual void cleanup() override;
    virtual void execute(GeometryRenderQueue& renderQueue, std::vector<RendererPtr> const& renderer)
        override;

  private:
    void applyDirectionalLights();
    void applyPointLights();

  private:
    Asset<ShaderProgram> m_dir_light_shader;
    Asset<ShaderProgram> m_point_light_shader;

    uint m_vao         = 0;
    uint m_vbo         = 0;
    uint m_buffer_size = 0;

    Scene*                                 m_scene                 = nullptr;
    DirectionalLightSource::ContainerType* m_dir_light_container   = nullptr;
    PointLightSource::ContainerType*       m_point_light_container = nullptr;
    PositionComponent::ContainerType*      m_position_container    = nullptr;
};

} // namespace bembel::graphics
