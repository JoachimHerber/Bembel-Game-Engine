module;
#include <array>
export module bembel.graphics.pipeline:ShadowMap;

import bembel.base;
import bembel.kernel;
import bembel.graphics.geometry;
import :RenderingPipeline;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export class ShadowMap {
    using RendererPtr = RenderingPipeline::RendererPtr;

  public:
    ShadowMap(Texture::Target target);
    ~ShadowMap();

    void setResolution(uint resolution);

    void updateLayer(
        In<GeometryRenderQueue>          render_queue,
        In<std::span<const RendererPtr>> renderer,
        In<mat4>                         view_proj,
        usize                            layer
    );

    Texture& getTexture() { return m_texture; }
    auto&    getTransforms() const { return m_transforms; }

    usize static constexpr MAX_NUM_LAYERS = 64;

  private:
    Texture                                       m_texture;
    uint                                          m_resolution;
    std::array<FrameBufferObject, MAX_NUM_LAYERS> m_fbos;
    std::array<mat4, MAX_NUM_LAYERS>              m_transforms;
};

} // namespace bembel::graphics