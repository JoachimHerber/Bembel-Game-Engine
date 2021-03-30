#include "./graphic-system.hpp"

#include <bembel/base/io/xml.hpp>
#include <bembel/base/logging/logger.hpp>
#include <bembel/kernel/assets/asset-manager.hpp>
#include <bembel/kernel/core/kernel.hpp>
#include <bembel/kernel/display/display-manager.hpp>
#include <bembel/kernel/display/window.hpp>
#include <bembel/kernel/events/display-events.hpp>
#include <bembel/kernel/open-gl.hpp>
#include <bembel/kernel/rendering/shader.hpp>

#include "./geometry/material.hpp"
#include "./geometry/mesh.hpp"
#include "./geometry/model.hpp"
#include "./geometry/renderer.hpp"
#include "./rendering-pipeline/deferred-lighting-stage.hpp"
#include "./rendering-pipeline/environment-map-reflection-stage.hpp"
#include "./rendering-pipeline/geometry-rendering-stage.hpp"
#include "./rendering-pipeline/rendering-pipeline.hpp"
#include "./texture-view.hpp"

namespace bembel::graphics {

GraphicSystem::GraphicSystem(kernel::Kernel& kernel)
: System(kernel, "Graphics")
, geometry_render_queue{kernel.getAssetManager()} {
    auto& asset_mgr = this->kernel.getAssetManager();
    asset_mgr.registerAssetType<Material>(this);
    asset_mgr.registerAssetType<GeometryMesh>();
    asset_mgr.registerAssetType<GeometryModel>();
    asset_mgr.registerAssetType<kernel::Shader>();
    asset_mgr.registerAssetType<kernel::ShaderProgram>();

    this->rendering_stage_factory.registerDefaultObjectGenerator<GeometryRenderingStage>(
        "DeferredGeometryStage");
    this->rendering_stage_factory.registerDefaultObjectGenerator<DeferredLightingStage>(
        "DeferredLightingStage");
    this->rendering_stage_factory.registerDefaultObjectGenerator<EnvironmentMapReflectionStage>(
        "EnvironmentMapReflectionStage");
}
GraphicSystem::~GraphicSystem() {
    // this->kernel.getEventManager().removeHandler<kernel::WindowUpdateEvent>(this);
    // this->kernel.getEventManager().removeHandler<kernel::FrameBufferResizeEvent>(this);
}

RenderingPipeline* GraphicSystem::createRenderingPipline() {
    this->pipelines.push_back(std::make_unique<RenderingPipeline>(*this));
    return this->pipelines.back().get();
}

const std::vector<std::shared_ptr<RenderingPipeline>>& GraphicSystem::getRenderingPipelines() {
    return this->pipelines;
}

const std::vector<std::shared_ptr<GeometryRendererBase>>& GraphicSystem::getRenderer() const {
    return this->renderer;
}

GeometryRendererBase* GraphicSystem::getRenderer(const std::string& name) const {
    auto it = this->renderer_map.find(name);
    if(it != this->renderer_map.end())
        return this->renderer[it->second].get();
    else
        return nullptr;
}

GraphicSystem::RendertingStageFactory& GraphicSystem::getRendertingStageFactory() {
    return this->rendering_stage_factory;
}

GeometryRenderQueue& GraphicSystem::getGeometryRenderQueue() {
    return this->geometry_render_queue;
}

bool GraphicSystem::configure(const base::xml::Element* properties) {
    if(!properties) return false;

    this->configureRenderer(properties->FirstChildElement("Renderer"));
    this->configurePipelines(properties->FirstChildElement("RenderingPipelines"));
    this->kernel.getEventManager().broadcast(InitGraphicResourcesEvent{});
    return true;
}

bool GraphicSystem::init() {
    for(auto& pipline : this->pipelines) pipline->init();

    this->kernel.getEventManager().broadcast(InitGraphicResourcesEvent{});
    return true;
}

void GraphicSystem::shutdown() {
    this->pipelines.clear();
    this->kernel.getEventManager().broadcast(CleanuptGraphicResourcesEvent{});
}

void GraphicSystem::update(double) {
    for(auto& pipline : this->pipelines) pipline->update();
}

void GraphicSystem::configureRenderer(const base::xml::Element* properties) {
    if(!properties) return;

    for(auto renderer_properties : base::xml::IterateChildElements(properties)) {
        RendererPtr renderer = DefaultGeometryRenderer::createRenderer(
            renderer_properties,
            this->kernel.getAssetManager(),
            unsigned int(this->renderer.size()));

        if(renderer) {
            std::string name;
            if(base::xml::getAttribute(renderer_properties, "name", name))
                this->renderer_map.emplace(name, this->renderer.size());

            this->renderer.push_back(std::move(renderer));
        }
    }
}

void GraphicSystem::configurePipelines(const base::xml::Element* properties) {
    if(!properties) return;

    for(auto pipeline_properties :
        base::xml::IterateChildElements(properties, "RenderingPipeline")) {
        auto pipline = std::make_unique<RenderingPipeline>(*this);
        if(!pipline->configure(pipeline_properties)) pipline.reset();

        this->pipelines.push_back(std::move(pipline));
    }
}

} // namespace bembel::graphics
