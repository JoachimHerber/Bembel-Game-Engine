﻿module;
#include <memory>
#include <string_view>
#include <utility>
module bembel.graphics;

import bembel.base;
import bembel.kernel;
import bembel.graphics.geometry;
import bembel.graphics.pipeline;

namespace bembel::graphics {
using namespace bembel::base;
using namespace bembel::kernel;

GraphicSystem::GraphicSystem(In<Engine*> engine) : System("Graphics"), m_engine{engine} {
    assets::registerAssetType<Material>();
    assets::registerAssetType<GeometryMesh>();
    assets::registerAssetType<GeometryModel>();
    assets::registerAssetType<Shader>();
    assets::registerAssetType<ShaderProgram>();

    using Stage = RenderingPipeline::Stage;
    Stage::registerStageType<GeometryRenderingStage>("DeferredGeometryStage");
    Stage::registerStageType<DeferredLightingStage>("DeferredLightingStage");
    Stage::registerStageType<EnvironmentMapReflectionStage>("EnvironmentMapReflectionStage");
}
GraphicSystem::~GraphicSystem() {
    events::removeHandler<WindowUpdateEvent>(this);
    events::removeHandler<FrameBufferResizeEvent>(this);
}

RenderingPipeline* GraphicSystem::createRenderingPipline() {
    m_pipelines.push_back(std::make_unique<RenderingPipeline>(m_engine->display));
    return m_pipelines.back().get();
}

std::vector<std::shared_ptr<RenderingPipeline>> const& GraphicSystem::getRenderingPipelines() {
    return m_pipelines;
}

std::vector<std::shared_ptr<GeometryRendererBase>> const& GraphicSystem::getRenderer() const {
    return m_renderer;
}

GeometryRendererBase* GraphicSystem::getRenderer(VertexAttribMask vertex_format) const {
    for(auto& it : m_renderer) {
        if(it->getRequiredVertexAttributes() == vertex_format) return it.get();
    }
    for(auto& it : m_renderer) {
        if((it->getRequiredVertexAttributes() & vertex_format) == it->getRequiredVertexAttributes())
            return it.get();
    }
    return nullptr;
}

bool GraphicSystem::configure(xml::Element const* properties) {
    if(!properties) return false;

    configureRenderer(properties->FirstChildElement("Renderer"));
    configurePipelines(properties->FirstChildElement("RenderingPipelines"));
    events::broadcast<InitGraphicResourcesEvent>();
    return true;
}

bool GraphicSystem::init() {
    for(auto& pipline : m_pipelines) pipline->init();

    events::broadcast<InitGraphicResourcesEvent>();
    return true;
}

void GraphicSystem::shutdown() {
    m_pipelines.clear();
    events::broadcast<CleanuptGraphicResourcesEvent>();
}

void GraphicSystem::update(double) {
    for(auto& pipline : m_pipelines) pipline->update(m_renderer);
}

void GraphicSystem::configureRenderer(xml::Element const* properties) {
    if(!properties) return;

    m_renderer.resize(3);
    for(auto renderer_properties : xml::IterateChildElements(properties)) {
        auto renderer = DefaultGeometryRenderer::createRenderer(renderer_properties);
        if(!renderer) return;

        m_renderer.push_back(std::move(renderer));
    }
}
void GraphicSystem::configurePipelines(xml::Element const* properties) {
    if(!properties) return;

    for(auto pipeline_properties : xml::IterateChildElements(properties, "RenderingPipeline")) {
        auto pipline = std::make_unique<RenderingPipeline>(m_engine->display);
        if(!pipline->configure(pipeline_properties)) pipline.reset();

        m_pipelines.push_back(std::move(pipline));
    }
}

} // namespace bembel::graphics
