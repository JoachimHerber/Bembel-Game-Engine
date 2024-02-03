module;
#include <memory>
#include <string>
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

    GraphicSystem(In<Engine*>);
    GraphicSystem(GraphicSystem const&)            = delete;
    GraphicSystem& operator=(GraphicSystem const&) = delete;
    ~GraphicSystem();

    std::vector<RendererPtr> const& getRenderer() const;
    GeometryRendererBase*           getRenderer(VertexAttribMask) const;

    RenderingPipeline*                       createRenderingPipline();
    std::vector<RenderingPipelinePtr> const& getRenderingPipelines();

    template <typename TRenderer, typename... TArgs>
    TRenderer* setRenderer(VertexAttribMask vertex_format, TArgs&&... args) {
        auto renderer = std::make_unique<TRenderer>(vertex_format, std::forward<TArgs>(args)...);

        TRenderer* ptr = renderer.get();
        for(auto& it : m_renderer) {
            if(it->getRequiredVertexAttributes() == vertex_format) {
                m_renderer[std::to_underlying(vertex_format)] = std::move(renderer);
                return ptr;
            }
        }
        m_renderer.push_back(std::move(renderer));
        return ptr;
    }

    virtual bool configure(xml::Element const*) override;

    virtual bool init() override;
    virtual void shutdown() override;
    virtual void update(double time_since_last_update) override;

  private:
    void configureRenderer(xml::Element const*);
    void configurePipelines(xml::Element const*);

  private:
    Engine* m_engine;

    std::vector<RendererPtr>          m_renderer;
    std::vector<RenderingPipelinePtr> m_pipelines;
};

export struct InitGraphicResourcesEvent {};
export struct CleanuptGraphicResourcesEvent {};

} // namespace bembel::graphics
