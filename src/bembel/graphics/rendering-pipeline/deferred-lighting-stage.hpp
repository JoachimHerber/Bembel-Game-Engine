#ifndef BEMBEL_GRAPHICS_RENDERING_PIPELINE_DEFERREDLIGHTINGSTAGE_HPP
#define BEMBEL_GRAPHICS_RENDERING_PIPELINE_DEFERREDLIGHTINGSTAGE_HPP

#include <bembel/base/io/xml.hpp>
#include <bembel/kernel/assets/asset.hpp>
#include <bembel/kernel/open-gl.hpp>
#include <bembel/kernel/rendering/frame-buffer-object.hpp>
#include <bembel/kernel/rendering/shader.hpp>
#include <bembel/kernel/rendering/texture.hpp>
#include <bembel/kernel/scene/position-component.hpp>
#include <bembel/kernel/scene/scene.hpp>

#include "./light-source.hpp"
#include "./rendering-stage.hpp"

namespace bembel::graphics {

class BEMBEL_API DeferredLightingStage : public RenderingStage {
  public:
    DeferredLightingStage(RenderingPipeline& pipline);
    ~DeferredLightingStage();

    void setDirLightShader(kernel::Asset<kernel::ShaderProgram>);
    void setPointLightShader(kernel::Asset<kernel::ShaderProgram>);

    virtual void setScene(kernel::Scene*) override;

    virtual void init() override;
    virtual void cleanup() override;
    virtual void execute() override;

    static std::unique_ptr<DeferredLightingStage> createInstance(
        const base::xml::Element*, RenderingPipeline&);

  private:
    void applyDirectionalLights();
    void applyPointLights();

  private:
    kernel::Asset<kernel::ShaderProgram> dir_light_shader;
    kernel::Asset<kernel::ShaderProgram> point_light_shader;

    GLuint   vao;
    GLuint   vbo;
    unsigned buffer_size;

    kernel::Scene*                            scene                 = nullptr;
    DirectionalLightSource::ContainerType*    dir_light_container   = nullptr;
    PointLightSource::ContainerType*          point_light_container = nullptr;
    kernel::PositionComponent::ContainerType* position_container    = nullptr;
};

} // namespace bembel::graphics
#endif // include guards
