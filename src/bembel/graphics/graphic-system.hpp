#ifndef BEMBEL_GRAPHICS_GRAPHICSSYSTEM_HPP
#define BEMBEL_GRAPHICS_GRAPHICSSYSTEM_HPP

#include <bembel/base/utils/factory.hpp>
#include <bembel/kernel/core/system.hpp>
#include <bembel/kernel/events/display-events.hpp>

#include "./geometry/render-queue.hpp"
#include "./rendering-pipeline/rendering-stage.hpp"

namespace bembel::graphics {

class RenderingPipeline;
class GeometryRendererBase;

class BEMBEL_API GraphicSystem : public kernel::System {
  public:
    using RendererPtr          = std::shared_ptr<GeometryRendererBase>;
    using RenderingPipelinePtr = std::shared_ptr<RenderingPipeline>;
    using RendertingStageFactory =
        base::Factory<RenderingStage, const base::xml::Element*, RenderingPipeline&>;

    GraphicSystem(kernel::Kernel&);
    GraphicSystem(const GraphicSystem&) = delete;
    GraphicSystem& operator=(const GraphicSystem&) = delete;
    ~GraphicSystem();

    const std::vector<RendererPtr>& getRenderer() const;
    GeometryRendererBase*           getRenderer(const std::string& name) const;

    RenderingPipeline*                       createRenderingPipline();
    const std::vector<RenderingPipelinePtr>& getRenderingPipelines();

    RendertingStageFactory& getRendertingStageFactory();
    GeometryRenderQueue&    getGeometryRenderQueue();

    virtual bool configure(const base::xml::Element*) override;

    virtual bool init() override;
    virtual void shutdown() override;
    virtual void update(double time_since_last_update) override;

  private:
    void configureRenderer(const base::xml::Element*);
    void configurePipelines(const base::xml::Element*);

  private:
    std::vector<RendererPtr>          renderer;
    std::vector<RenderingPipelinePtr> pipelines;

    std::map<std::string, size_t> renderer_map;

    RendertingStageFactory rendering_stage_factory;
    GeometryRenderQueue    geometry_render_queue;
};

struct InitGraphicResourcesEvent {};

struct CleanuptGraphicResourcesEvent {};

} // namespace bembel::graphics
#endif // include guards
