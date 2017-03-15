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
{
	_kernel->GetAssetManager()->RegisterAssetType<Material>(this);
	_kernel->GetAssetManager()->RegisterAssetType<GeometryMesh>();
	_kernel->GetAssetManager()->RegisterAssetType<GeometryModel>();

	_kernel->GetEventManager()->AddHandler<WindowUpdateEvent>(this);
	_kernel->GetEventManager()->AddHandler<FrameBufferResizeEvent>(this);

	_renderingStageFactory.RegisterDefaultObjectGenerator
		<GeometryRenderingStage>("DeferredGeometryStage");
	_renderingStageFactory.RegisterDefaultObjectGenerator
		<DeferredLightingStage>("DeferredLightingStage");
	_renderingStageFactory.RegisterDefaultObjectGenerator
		<EnvironmentMapReflectionStage>("EnvironmentMapReflectionStage");

	_geometryRenderQueue.SetAssetMannager(kernel->GetAssetManager());
}
GraphicSystem::~GraphicSystem()
{
	_kernel->GetEventManager()->RemoveHandler<WindowUpdateEvent>(this);
	_kernel->GetEventManager()->RemoveHandler<FrameBufferResizeEvent>(this);
}

Viewport* GraphicSystem::CreateViewPort(unsigned windowID /*= 0*/)
{
	if (_viewports.size() <= windowID)
		_viewports.resize(windowID+1);

	_viewports[windowID].push_back(std::make_unique<Viewport>());
	return _viewports[windowID].back().get();
}

const std::vector<std::shared_ptr<Viewport>>&
	GraphicSystem::GetViewports(unsigned windowID /*= 0*/)
{
	if (_viewports.size() <= windowID)
		_viewports.resize(windowID+1);
	return _viewports[windowID];
}

void GraphicSystem::UpdateViewports()
{
	for (size_t n = 0; n < _viewports.size(); ++n)
	{
		auto window = _kernel->GetDisplayManager()->GetWindow(n);
		if (!window)
			continue;

		auto& windowViewports = _viewports[n];
		for (auto& viewport : windowViewports)
		{
			viewport->UpdatePosition(window->GetFrameBufferSize());
			viewport->UpdateSize(window->GetFrameBufferSize());
		}
	}
}

RenderingPipeline* GraphicSystem::CreateRenderingPipline()
{
	_pipelines.push_back(std::make_unique<RenderingPipeline>(this));
	return _pipelines.back().get();
}

const std::vector<std::shared_ptr<RenderingPipeline>>&
	GraphicSystem::GetRenderingPiplies()
{
	return _pipelines;
}

const std::vector<std::shared_ptr<GeometryRendererBase>>& GraphicSystem::GetRenderer() const
{
	return _renderer;
}

GeometryRendererBase* GraphicSystem::GetRenderer( const std::string& name ) const
{
	auto it = _rendererMap.find( name );
	if(it != _rendererMap.end())
		return _renderer[it->second].get();
	else
		return nullptr;
}

GraphicSystem::RendertingStageFactory& GraphicSystem::GetRendertingStageFactory()
{
	return _renderingStageFactory;
}

GeometryRenderQueue& GraphicSystem::GetGeometryRenderQueue()
{
	return _geometryRenderQueue;
}

bool GraphicSystem::Configure(const xml::Element* properties)
{
	if (!properties)
		return false;

	ConfigureRenderer(properties->FirstChildElement("Renderer"));
	ConfigurePipelines(properties->FirstChildElement("RenderingPipelines"));
	ConfigureViewports(properties->FirstChildElement("Viewports"));

	_kernel->GetEventManager()->Broadcast(InitGraphicResourcesEvent{});
	return true;
}

bool GraphicSystem::Init()
{
	for (auto& pipline : _pipelines)
		pipline->Init();

	UpdateViewports();

	_kernel->GetEventManager()->Broadcast(InitGraphicResourcesEvent{});
	return true;
}

void GraphicSystem::Shutdown()
{
	_pipelines.clear();
	_kernel->GetEventManager()->Broadcast(CleanuptGraphicResourcesEvent{});
}

void GraphicSystem::Update(double)
{
	for (auto& pipline : _pipelines)
		pipline->Update();
}

void GraphicSystem::HandleEvent(const WindowUpdateEvent& event)
{
	unsigned windowID = event.window->GetWindowID();
	if (_viewports.size() <= windowID)
		return;

	auto& windowViewports = _viewports[windowID];
	for (auto& viewport : windowViewports )
	{
		if(!(viewport->IsEnabled()))
			continue;

		auto view = viewport->GetView();
		if(!view)
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
	unsigned windowID = event.window->GetWindowID();
	if (_viewports.size() <= windowID)
		return;

	auto& windowViewports = _viewports[windowID];
	for (auto& viewport : windowViewports)
	{
		viewport->UpdatePosition(event.size);
		viewport->UpdateSize(event.size);
	}
}

void GraphicSystem::ConfigureRenderer(const xml::Element* properties)
{
	if (!properties)
		return;

	for (auto rendererProperties : xml::IterateChildElements(properties))
	{
		RendererPtr renderer =
			DefaultGeometryRenderer::CreateRenderer(
				rendererProperties, _renderer.size());

		if( renderer )
		{
			std::string name;
			if( xml::GetAttribute( rendererProperties, "name", name ) )
				_rendererMap.emplace( name, _renderer.size() );

			_renderer.push_back( std::move( renderer ) );
		}
	}
}

void GraphicSystem::ConfigurePipelines(const xml::Element* properties)
{
	if (!properties)
		return;

	for( auto pipelineProperties : xml::IterateChildElements(properties, "RenderingPipeline"))
	{
		auto pipline = std::make_unique<RenderingPipeline>(this);
		if (!pipline->Configure(pipelineProperties))
			pipline.reset();

		_pipelines.push_back(std::move(pipline));
	}
}

void GraphicSystem::ConfigureViewports(const xml::Element* properties)
{
	if (!properties)
		return;

	for (auto viewportProperties : xml::IterateChildElements(properties, "Viewport"))
	{
		int windowID = 0;
		xml::GetAttribute(viewportProperties, "window", windowID);
		auto viewport = CreateViewPort(windowID);

		glm::vec2 relPos(0, 0);
		glm::vec2 relSize(1, 1);
		glm::vec2 posOffset(0, 0);
		glm::vec2 sizeOffset(0, 0);
		xml::GetAttribute(viewportProperties, "position", relPos);
		xml::GetAttribute(viewportProperties, "size", relSize);
		xml::GetAttribute(viewportProperties, "position_offset", posOffset);
		xml::GetAttribute(viewportProperties, "size_offset", sizeOffset);
		viewport->SetRelativPosition(relPos);
		viewport->SetRelativSize(relSize);
		viewport->SetPositionOffset(posOffset);
		viewport->SetSizeOffset(sizeOffset);
		auto window = _kernel->GetDisplayManager()->GetWindow(windowID);
		if (window)
		{
			glm::vec2 fbSize = window->GetFrameBufferSize();
			viewport->UpdatePosition(fbSize);
			viewport->UpdateSize(fbSize);
		}

		for (auto viewProperties : xml::IterateChildElements(viewportProperties))
		{
			std::string viewType = viewProperties->Value();
			if (viewType == "PipelineResultView")
			{
				int pipeline = 0, view = 0;
				xml::GetAttribute(viewProperties, "pipeline", pipeline);
				xml::GetAttribute(viewProperties, "view", view);
				viewport->SetView(_pipelines[pipeline]->GetViews()[view]);
			}
		}
	}
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/