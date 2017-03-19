/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "graphic-system.h"
#include "viewport.h"
#include "texture-view.h"
#include "geometry/material.h"
#include "geometry/geometry-mesh.h"
#include "geometry/geometry-model.h"
#include "geometry/geometry-renderer.h"
#include "rendering-pipeline/rendering-pipeline.h"
#include "rendering-pipeline/geometry-rendering-stage.h"
#include "rendering-pipeline/deferred-lighting-stage.h"
#include "rendering-pipeline/environment-map-reflection-stage.h"

#include <bembel-open-gl.h>
#include <bembel-base/xml.h>
#include <bembel-base/logging/logger.h>
#include <bembel-kernel/kernel.h>
#include <bembel-kernel/display/display-manager.h>
#include <bembel-kernel/display/window.h>
#include <bembel-kernel/events/display-events.h>
#include <bembel-kernel/assets/asset-manager.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(InitGraphicResourcesEvent);
BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(CleanuptGraphicResourcesEvent);

GraphicSystem::GraphicSystem(Kernel* kernel)
	: System(kernel, "Graphics")
	, geometry_render_queue_(kernel->GetAssetManager())
{
	kernel_->GetAssetManager()->RegisterAssetType<Material>(this);
	kernel_->GetAssetManager()->RegisterAssetType<GeometryMesh>();
	kernel_->GetAssetManager()->RegisterAssetType<GeometryModel>();

	kernel_->GetEventManager()->AddHandler<WindowUpdateEvent>(this);
	kernel_->GetEventManager()->AddHandler<FrameBufferResizeEvent>(this);

	rendering_stage_factory_.RegisterDefaultObjectGenerator
		<GeometryRenderingStage>("DeferredGeometryStage");
	rendering_stage_factory_.RegisterDefaultObjectGenerator
		<DeferredLightingStage>("DeferredLightingStage");
	rendering_stage_factory_.RegisterDefaultObjectGenerator
		<EnvironmentMapReflectionStage>("EnvironmentMapReflectionStage");
}
GraphicSystem::~GraphicSystem()
{
	kernel_->GetEventManager()->RemoveHandler<WindowUpdateEvent>(this);
	kernel_->GetEventManager()->RemoveHandler<FrameBufferResizeEvent>(this);
}

Viewport* GraphicSystem::CreateViewPort(unsigned windowID /*= 0*/)
{
	if( viewports_.size() <= windowID )
		viewports_.resize(windowID+1);

	viewports_[windowID].push_back(std::make_unique<Viewport>());
	return viewports_[windowID].back().get();
}

const std::vector<std::shared_ptr<Viewport>>&
GraphicSystem::GetViewports(unsigned windowID /*= 0*/)
{
	if( viewports_.size() <= windowID )
		viewports_.resize(windowID+1);
	return viewports_[windowID];
}

void GraphicSystem::UpdateViewports()
{
	for( size_t n = 0; n < viewports_.size(); ++n )
	{
		auto window = kernel_->GetDisplayManager()->GetWindow(n);
		if( !window )
			continue;

		auto& windowViewports = viewports_[n];
		for( auto& viewport : windowViewports )
		{
			viewport->UpdatePosition(window->GetFrameBufferSize());
			viewport->UpdateSize(window->GetFrameBufferSize());
		}
	}
}

RenderingPipeline* GraphicSystem::CreateRenderingPipline()
{
	pipelines_.push_back(std::make_unique<RenderingPipeline>(this));
	return pipelines_.back().get();
}

const std::vector<std::shared_ptr<RenderingPipeline>>&
GraphicSystem::GetRenderingPiplies()
{
	return pipelines_;
}

const std::vector<std::shared_ptr<GeometryRendererBase>>& GraphicSystem::GetRenderer() const
{
	return renderer_;
}

GeometryRendererBase* GraphicSystem::GetRenderer(const std::string& name) const
{
	auto it = renderer_map_.find(name);
	if( it != renderer_map_.end() )
		return renderer_[it->second].get();
	else
		return nullptr;
}

GraphicSystem::RendertingStageFactory& GraphicSystem::GetRendertingStageFactory()
{
	return rendering_stage_factory_;
}

GeometryRenderQueue& GraphicSystem::GetGeometryRenderQueue()
{
	return geometry_render_queue_;
}

bool GraphicSystem::Configure(const xml::Element* properties)
{
	if( !properties )
		return false;

	ConfigureRenderer(properties->FirstChildElement("Renderer"));
	ConfigurePipelines(properties->FirstChildElement("RenderingPipelines"));
	ConfigureViewports(properties->FirstChildElement("Viewports"));

	kernel_->GetEventManager()->Broadcast(InitGraphicResourcesEvent{});
	return true;
}

bool GraphicSystem::Init()
{
	for( auto& pipline : pipelines_ )
		pipline->Init();

	UpdateViewports();

	kernel_->GetEventManager()->Broadcast(InitGraphicResourcesEvent{});
	return true;
}

void GraphicSystem::Shutdown()
{
	pipelines_.clear();
	kernel_->GetEventManager()->Broadcast(CleanuptGraphicResourcesEvent{});
}

void GraphicSystem::Update(double)
{
	for( auto& pipline : pipelines_ )
		pipline->Update();
}

void GraphicSystem::HandleEvent(const WindowUpdateEvent& event)
{
	unsigned window_id = event.window->GetWindowID();
	if( viewports_.size() <= window_id )
		return;

	auto& windowViewports = viewports_[window_id];
	for( auto& viewport : windowViewports )
	{
		if( !(viewport->IsEnabled()) )
			continue;

		auto view = viewport->GetView();
		if( !view )
			continue;

		glViewport(
			viewport->GetPosition().x,
			viewport->GetPosition().y,
			viewport->GetSize().x,
			viewport->GetSize().y
		);
		view->Draw();
	}
}

void GraphicSystem::HandleEvent(const FrameBufferResizeEvent& event)
{
	unsigned window_id = event.window->GetWindowID();
	if( viewports_.size() <= window_id )
		return;

	auto& window_viewports = viewports_[window_id];
	for( auto& viewport : window_viewports )
	{
		viewport->UpdatePosition(event.size);
		viewport->UpdateSize(event.size);
	}
}

void GraphicSystem::ConfigureRenderer(const xml::Element* properties)
{
	if( !properties )
		return;

	for( auto rendererProperties : xml::IterateChildElements(properties) )
	{
		RendererPtr renderer =
			DefaultGeometryRenderer::CreateRenderer(
				rendererProperties, renderer_.size());

		if( renderer )
		{
			std::string name;
			if( xml::GetAttribute(rendererProperties, "name", name) )
				renderer_map_.emplace(name, renderer_.size());

			renderer_.push_back(std::move(renderer));
		}
	}
}

void GraphicSystem::ConfigurePipelines(const xml::Element* properties)
{
	if( !properties )
		return;

	for( auto pipelineProperties : xml::IterateChildElements(properties, "RenderingPipeline") )
	{
		auto pipline = std::make_unique<RenderingPipeline>(this);
		if( !pipline->Configure(pipelineProperties) )
			pipline.reset();

		pipelines_.push_back(std::move(pipline));
	}
}

void GraphicSystem::ConfigureViewports(const xml::Element* properties)
{
	if( !properties )
		return;

	for( auto viewport_properties : xml::IterateChildElements(properties, "Viewport") )
	{
		int window_id = 0;
		xml::GetAttribute(viewport_properties, "window", window_id);
		auto viewport = CreateViewPort(window_id);

		glm::vec2 position(0, 0);
		glm::vec2 size(1, 1);
		glm::vec2 position_offset(0, 0);
		glm::vec2 size_offset(0, 0);
		xml::GetAttribute(viewport_properties, "position", position);
		xml::GetAttribute(viewport_properties, "size", size);
		xml::GetAttribute(viewport_properties, "position_offset", position_offset);
		xml::GetAttribute(viewport_properties, "size_offset", size_offset);
		viewport->SetRelativPosition(position);
		viewport->SetRelativSize(size);
		viewport->SetPositionOffset(position_offset);
		viewport->SetSizeOffset(size_offset);
		auto window = kernel_->GetDisplayManager()->GetWindow(window_id);
		if( window )
		{
			glm::vec2 frame_buffer_size = window->GetFrameBufferSize();
			viewport->UpdatePosition(frame_buffer_size);
			viewport->UpdateSize(frame_buffer_size);
		}

		for( auto view_properties : xml::IterateChildElements(viewport_properties) )
		{
			std::string viewType = view_properties->Value();
			if( viewType == "PipelineResultView" )
			{
				int pipeline = 0, view = 0;
				xml::GetAttribute(view_properties, "pipeline", pipeline);
				xml::GetAttribute(view_properties, "view", view);
				viewport->SetView(pipelines_[pipeline]->GetViews()[view]);
			}
		}
	}
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
