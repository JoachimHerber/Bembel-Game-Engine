/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "GraphicSystem.h"
#include "Viewport.h"
#include "TextureView.h"
#include "RenderingPipeline/RenderingPipeline.h"
#include "RenderingPipeline/DeferredGeometryStage.h"
#include "RenderingPipeline/DeferredLightingStage.h"

#include <BembelOpenGL.h>
#include <BembelBase/XML.h>
#include <BembelBase/Logging/Logger.h>
#include <BembelDisplay/Window.h>
#include <BembelDisplay/Events.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(InitGraphicResourcesEvent);
BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(CleanuptGraphicResourcesEvent);

GraphicSystem::GraphicSystem(std::shared_ptr<EventManager> eventMgr)
	: System(eventMgr, "Graphics")
{
	_eventMgr->AddHandler<WindowUpdateEvent>(this);

	_renderingStageFactory.RegisterDefaultObjectGenerator
		<DeferredGeometryStage>("DeferredGeometryStage");
	_renderingStageFactory.RegisterDefaultObjectGenerator
		<DeferredLightingStage>("DeferredLightingStage");
}
GraphicSystem::~GraphicSystem()
{
	_eventMgr->RemoveHandler<WindowUpdateEvent>(this);
}

std::shared_ptr<Viewport> GraphicSystem::CreateViewPort(unsigned windowID /*= 0*/)
{
	if (_viewports.size() <= windowID)
		_viewports.resize(windowID+1);

	_viewports[windowID].push_back(std::make_shared<Viewport>());
	return _viewports[windowID].back();
}

std::vector<std::shared_ptr<Viewport>>& 
	GraphicSystem::GetViewports(unsigned windowID /*= 0*/)
{
	if (_viewports.size() <= windowID)
		_viewports.resize(windowID+1);
	return _viewports[windowID];
}

GraphicSystem::RenderingPipelinePtr GraphicSystem::CreateRenderingPipline()
{
	RenderingPipelinePtr pipline = std::make_shared<RenderingPipeline>(this);
	_pipelines.push_back(pipline);
	return pipline;
}

std::vector<std::shared_ptr<RenderingPipeline>>& GraphicSystem::GetRenderingPiplies()
{
	return _pipelines;
}

GraphicSystem::RendererFactory& GraphicSystem::GetRendererFactory()
{
	return _rendererFactory;
}

GraphicSystem::RendertingSrageFactory& GraphicSystem::GetRendertingSrageFactory()
{
	return _renderingStageFactory;
}

bool GraphicSystem::Init()
{
	for (auto pipline : _pipelines)
		pipline->Init();
	_eventMgr->Broadcast(InitGraphicResourcesEvent{});
	return true;
}

bool GraphicSystem::Init(const xml::Element* properties)
{
	if (!properties)
		return false;

	InitPipelines(properties->FirstChildElement("RenderingPipelines"));
	InitViewports(properties->FirstChildElement("Viewports"));

	_eventMgr->Broadcast(InitGraphicResourcesEvent{});
	return true;
}

void GraphicSystem::Shutdown()
{
	_pipelines.clear();
	_eventMgr->Broadcast(CleanuptGraphicResourcesEvent{});
}

void GraphicSystem::Update(double)
{
	for (auto pipline : _pipelines)
		pipline->Update();
}

void GraphicSystem::HandleEvent(const WindowUpdateEvent& event)
{
	unsigned windowID = event.window->GetWindowID();
	if (_viewports.size() <= windowID)
		return;

	auto& windowViewports = _viewports[windowID];
	for (auto viewport : windowViewports )
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

void GraphicSystem::InitPipelines(const xml::Element* properties)
{
	if (!properties)
		return;

	for( auto pipelineProperties : xml::IterateChildElements(properties, "RenderingPipeline"))
	{
		auto pipline = std::make_shared<RenderingPipeline>(this);
		if (!pipline->Init(pipelineProperties))
			pipline.reset();

		_pipelines.push_back(pipline);
	}
}

void GraphicSystem::InitViewports(const xml::Element* properties)
{
	if (!properties)
		return;

	for (auto viewportProperties : xml::IterateChildElements(properties, "Viewport"))
	{
		int window = 0;
		xml::GetAttribute(viewportProperties, "window", window);
		auto viewport = CreateViewPort(window);

		int x=0, y=0, w=1, h=1;
		xml::GetAttribute(viewportProperties, "x", x);
		xml::GetAttribute(viewportProperties, "y", y);
		xml::GetAttribute(viewportProperties, "width", w);
		xml::GetAttribute(viewportProperties, "height", h);
		viewport->SetPosition(glm::ivec2(x, y));
		viewport->SetSize(glm::ivec2(w, h));
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
