export module bembel.graphics.pipeline:EnvironmentMapReflectionStage;

import bembel.base;
import bembel.kernel;
import bembel.graphics.geometry;
import :RenderingPipeline;
import :LightSource;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export class EnvironmentMapReflectionStage : public RenderingPipeline::Stage {
  public:
    EnvironmentMapReflectionStage(RenderingPipeline& pipline);
    ~EnvironmentMapReflectionStage();

    bool initEnvironmentMap(
        std::string_view left,
        std::string_view right,
        std::string_view bottom,
        std::string_view top,
        std::string_view back,
        std::string_view front
    );

    void setShader(Asset<ShaderProgram>);

    void setOutputTexture(std::string_view);

    virtual bool configure(xml::Element const*) override;
    virtual void init() override;
    virtual void cleanup() override;
    virtual void execute(In<std::span<const RendererPtr>> renderer) override;

  private:
    std::unique_ptr<Texture> m_environment_map;

    kernel::Asset<ShaderProgram> m_shader_program;
};

} // namespace bembel::graphics
