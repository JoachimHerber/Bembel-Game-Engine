module;
#include "bembel/pch.h"
export module bembel.graphics:System;

import bembel.base;
import bembel.kernel;
import bembel.graphics.geometry;
import bembel.graphics.pipeline;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

export class GraphicSystem : public kernel::System {
  public:
    using RendererPtr          = std::shared_ptr<GeometryRendererBase>;
    using RenderingPipelinePtr = std::shared_ptr<RenderingPipeline>;
    using RendertingStageFactory =
        Factory<RenderingPipeline::Stage, xml::Element const*, RenderingPipeline&>;

    GraphicSystem(Engine&);
    GraphicSystem(GraphicSystem const&)            = delete;
    GraphicSystem& operator=(GraphicSystem const&) = delete;
    ~GraphicSystem();

    std::vector<RendererPtr> const& getRenderer() const;
    GeometryRendererBase*           getRenderer(std::string_view name) const;

    RenderingPipeline*                       createRenderingPipline();
    std::vector<RenderingPipelinePtr> const& getRenderingPipelines();

    GeometryRenderQueue& getGeometryRenderQueue() { return m_geometry_render_queue; }

    virtual bool configure(xml::Element const*) override;

    virtual bool init() override;
    virtual void shutdown() override;
    virtual void update(double time_since_last_update) override;

  private:
    void configureRenderer(xml::Element const*);
    void configurePipelines(xml::Element const*);

  private:
    Engine& m_engine;

    std::vector<RendererPtr>          m_renderer;
    std::vector<RenderingPipelinePtr> m_pipelines;

    Dictionary<size_t> m_renderer_map;

    GeometryRenderQueue m_geometry_render_queue;
};

export struct InitGraphicResourcesEvent {};
export struct CleanuptGraphicResourcesEvent {};

} // namespace bembel::graphics
