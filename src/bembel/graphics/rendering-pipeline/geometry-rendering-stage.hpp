#ifndef BEMBEL_GRAPHICS_RENDERING_PIPELINE_GEOMETRYRENDERINGSTAGE_HPP
#define BEMBEL_GRAPHICS_RENDERING_PIPELINE_GEOMETRYRENDERINGSTAGE_HPP

#include <bembel/base/io/xml.hpp>
#include <bembel/kernel/open-gl.hpp>
#include <bembel/kernel/rendering/frame-buffer-object.hpp>
#include <bembel/kernel/rendering/texture.hpp>
#include <bembel/kernel/scene/position-component.hpp>
#include <bembel/kernel/scene/rotation-component.hpp>

#include "../geometry/geometry.hpp"
#include "./rendering-stage.hpp"

namespace bembel::graphics {

class BEMBEL_API GeometryRenderingStage : public RenderingStage {
  public:
    GeometryRenderingStage(RenderingPipeline& pipline);
    ~GeometryRenderingStage();

    virtual void execute() override;

    void setScene(kernel::Scene*) override;

    static std::unique_ptr<GeometryRenderingStage> createInstance(
        const base::xml::Element*, RenderingPipeline&);

  private:
    kernel::Scene*                            scene               = nullptr;
    GeometryComponent::ContainerType*         geometry_components = nullptr;
    kernel::PositionComponent::ContainerType* position_components = nullptr;
    kernel::RotationComponent::ContainerType* rotation_components = nullptr;
};

} // namespace bembel::graphics
#endif // include guards
