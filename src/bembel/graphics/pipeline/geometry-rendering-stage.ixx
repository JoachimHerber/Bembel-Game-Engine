module;
#include <filesystem>
export module bembel.graphics.pipeline:GeometryRenderingStage;

import bembel.base;
import bembel.kernel;
import bembel.graphics.geometry;
import :RenderingPipeline;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export class GeometryRenderingStage : public RenderingPipeline::Stage {
  public:
    GeometryRenderingStage(RenderingPipeline& pipline);
    ~GeometryRenderingStage();

    virtual bool configure(xml::Element const*) override;
    virtual void setScene(Scene*) override;
    virtual void execute(In<std::span<const RendererPtr>> renderer) override;

    using RenderingPipeline::Stage::setColorOutputTexture;
    using RenderingPipeline::Stage::setDepthOutputTexture;

  private:
    Scene*                                   m_scene      = nullptr;
    ComponentMetaData<Geometry>::Container*  m_geometrys  = nullptr;
    ComponentMetaData<Transform>::Container* m_transforms = nullptr;

    GeometryRenderQueue m_render_queue;
};

} // namespace bembel::graphics
