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
    virtual void execute(GeometryRenderQueue& renderQueue, std::vector<RendererPtr> const& renderer)
        override;

  private:
    Scene*                        m_scene               = nullptr;
    GeometryComponent::Container* m_geometry_components = nullptr;
    PositionComponent::Container* m_position_components = nullptr;
    RotationComponent::Container* m_rotation_components = nullptr;
};

} // namespace bembel::graphics
