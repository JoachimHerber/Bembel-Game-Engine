module;
#include <string>
export module bembel.examples.gm_helper:RenderingStages;

import bembel;
import :Components;

namespace bembel::examples::gm_helper {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;

export class SpriteRenderingStage : public RenderingPipeline::Stage {
  public:
    SpriteRenderingStage(RenderingPipeline& pipline) : RenderingPipeline::Stage(pipline) {}
    ~SpriteRenderingStage() = default;

    using RenderingPipeline::Stage::setColorOutputTexture;
    using RenderingPipeline::Stage::setDepthOutputTexture;

    virtual bool configure(xml::Element const*) override;

    virtual void init() override;
    virtual void cleanup() override;

    virtual void setScene(Scene* scene) override { m_scene = scene; }
    virtual void execute(In<std::span<const RendererPtr>> renderer) override;

  private:
    Asset<ShaderProgram> m_shader_program;

    Scene* m_scene = nullptr;
};

export class TokenRenderingStage : public RenderingPipeline::Stage {
  public:
    TokenRenderingStage(RenderingPipeline& pipline) : RenderingPipeline::Stage(pipline) {}
    ~TokenRenderingStage() = default;

    using RenderingPipeline::Stage::setColorOutputTexture;
    using RenderingPipeline::Stage::setDepthOutputTexture;

    virtual bool configure(xml::Element const*) override;

    virtual void init() override;
    virtual void cleanup() override;

    virtual void setScene(Scene* scene) override { m_scene = scene; }
    virtual void execute(In<std::span<const RendererPtr>> renderer) override;

  private:
    Asset<ShaderProgram> m_shader_program;
    Asset<Texture>       m_overlay;
    Asset<Texture>       m_mask;

    Scene* m_scene = nullptr;

    bool m_show_hidden_tokens = false;
};

export class LightingOverlayStage : public RenderingPipeline::Stage {
  public:
    LightingOverlayStage(RenderingPipeline& pipline) : RenderingPipeline::Stage(pipline) {}
    ~LightingOverlayStage() {}

    using RenderingPipeline::Stage::setColorOutputTexture;

    virtual bool configure(xml::Element const*) override;

    virtual void init() override;
    virtual void cleanup() override;

    virtual void setScene(Scene* scene) override { m_scene = scene; }
    virtual void execute(In<std::span<const RendererPtr>> renderer) override;

  private:
    Asset<ShaderProgram> m_shader_program;

    Scene* m_scene = nullptr;
};

export class ApplyLightingOverlayStage : public RenderingPipeline::Stage {
  public:
    ApplyLightingOverlayStage(RenderingPipeline& pipline) : RenderingPipeline::Stage(pipline) {}
    ~ApplyLightingOverlayStage() {}

    using RenderingPipeline::Stage::setColorOutputTexture;
    using RenderingPipeline::Stage::setInputTextures;

    virtual bool configure(xml::Element const*) override;

    virtual void init() override;
    virtual void cleanup() override;

    virtual void setScene(Scene*) override{};
    virtual void execute(In<std::span<const RendererPtr>> renderer) override;

  private:
    Asset<ShaderProgram> m_shader_program;

    float m_intensity = 1.0f;
};
} // namespace bembel::examples::gm_helper
