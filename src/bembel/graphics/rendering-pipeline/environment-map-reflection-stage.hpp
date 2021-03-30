#ifndef BEMBEL_GRAPHICS_RENDERING_PIPELINE_ENVIRONMENTMAPREFLECTIONSTAGE_HPP
#define BEMBEL_GRAPHICS_RENDERING_PIPELINE_ENVIRONMENTMAPREFLECTIONSTAGE_HPP

#include <bembel/base/io/xml.hpp>
#include <bembel/kernel/assets/asset.hpp>
#include <bembel/kernel/open-gl.hpp>
#include <bembel/kernel/rendering/frame-buffer-object.hpp>
#include <bembel/kernel/rendering/shader.hpp>
#include <bembel/kernel/rendering/texture.hpp>

#include "./rendering-stage.hpp"

namespace bembel::graphics {

class BEMBEL_API EnvironmentMapReflectionStage : public RenderingStage {
  public:
    EnvironmentMapReflectionStage(RenderingPipeline& pipline);
    ~EnvironmentMapReflectionStage();

    bool initEnvironmentMap(
        const std::string& left,
        const std::string& right,
        const std::string& bottom,
        const std::string& top,
        const std::string& back,
        const std::string& front);

    void setShader(kernel::Asset<kernel::ShaderProgram>);

    void setOutputTexture(const std::string&);

    virtual void init() override;
    virtual void cleanup() override;
    virtual void execute() override;

    static std::unique_ptr<EnvironmentMapReflectionStage> createInstance(
        const base::xml::Element*, RenderingPipeline&);

  private:
    std::unique_ptr<kernel::Texture> environment_map;

    kernel::Asset<kernel::ShaderProgram> shader_program;
};

} // namespace bembel::graphics
#endif // include guards
