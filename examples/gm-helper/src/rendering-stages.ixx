export module bembel.examples.gm_helper:RenderingStages;

import std;
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
    void drawTokens();
    void drawAreaMarker();

  private:
    Asset<ShaderProgram> m_token_shader;
    Asset<ShaderProgram> m_area_shader;
    Asset<Texture>       m_overlay;
    Asset<Texture>       m_mask;

    Scene* m_scene = nullptr;

    bool m_show_hidden_tokens = false;
};

export class CreateLightingOverlayStage : public RenderingPipeline::Stage {
  public:
    CreateLightingOverlayStage(RenderingPipeline& pipline) : RenderingPipeline::Stage(pipline) {}
    ~CreateLightingOverlayStage() {}

    using RenderingPipeline::Stage::setColorOutputTexture;

    virtual bool configure(xml::Element const*) override;

    virtual void init() override;
    virtual void cleanup() override;

    virtual void setScene(Scene* scene) override { m_scene = scene; }
    virtual void execute(In<std::span<const RendererPtr>> renderer) override;

  private:
    void drawLights();
    void drawShadow();

  private:
    Asset<ShaderProgram> m_light_shader;
    Asset<ShaderProgram> m_shadow_shader;

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

export class InitiativeOverlayStage : public RenderingPipeline::Stage {
  public:
    InitiativeOverlayStage(RenderingPipeline& pipline) : RenderingPipeline::Stage(pipline) {}
    ~InitiativeOverlayStage() {}

    using RenderingPipeline::Stage::setColorOutputTexture;
    using RenderingPipeline::Stage::setInputTextures;

    virtual bool configure(xml::Element const*) override;

    virtual void init() override;
    virtual void cleanup() override;

    virtual void setScene(Scene* scene) override { m_scene = scene; }
    virtual void execute(In<std::span<const RendererPtr>> renderer) override;

  private:
    Asset<ShaderProgram> m_shader_program;
    Asset<Texture>       m_overlay;
    Asset<Texture>       m_mask;

    vec2 m_size{128, 128};
    vec2 m_size_active{200, 200};
    vec3 m_margin;

    Scene* m_scene = nullptr;
};

} // namespace bembel::examples::gm_helper
