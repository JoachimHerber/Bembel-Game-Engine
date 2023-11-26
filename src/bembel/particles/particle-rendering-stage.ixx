module;
#include <vector>
export module bembel.particles:RenderingStage;

import bembel.base;
import bembel.kernel;
import bembel.graphics;

namespace bembel::particles {
using namespace bembel::base;
using namespace bembel::kernel;
using namespace bembel::graphics;

export class ParticleRenderingStage : public RenderingPipeline::Stage {
  public:
    ParticleRenderingStage(RenderingPipeline& pipline);
    ~ParticleRenderingStage();

    virtual bool configure(xml::Element const*) override;
    virtual void setScene(Scene*) override;

    virtual void init() override;
    virtual void cleanup() override;
    virtual void execute(In<std::span<const RendererPtr>>) override;

  private:
    struct TexCoords {
        u16 min_u;
        u16 min_v;
        u16 max_u;
        u16 max_v;

        TexCoords(vec4 v)
          : min_u{u16(0xffff * v.x)}
          , min_v{u16(0xffff * v.y)}
          , max_u{u16(0xffff * v.z)}
          , max_v{u16(0xffff * v.w)} {}
    };
    struct ParticleRenderData {
        vec3      position;   //   12 Byte
        float     size;       // +  4 Byte -> 16 Byte;
        TexCoords tex_coords; // +  8 Byte -> 24 Byte;
        ColorRGBA color;      // +  4 Byte -> 28 Byte;
        float     dist;       // +  4 Byte -> 32 Byte;
    };

    Scene*                          m_scene = nullptr;
    uint                            m_vao   = 0;
    uint                            m_vbo   = 0;
    std::vector<ParticleRenderData> m_data;
    Asset<ShaderProgram>            m_shader;
    Asset<Texture>                  m_texture;
};

} // namespace bembel::particles
