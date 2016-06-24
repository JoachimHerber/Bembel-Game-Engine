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
#include <BembelKernel/Kernel.h>
#include <BembelKernel/Display/DisplayManager.h>
#include <BembelKernel/Display/Window.h>
#include <BembelKernel/Events/DisplayEvents.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(InitGraphicResourcesEvent);
BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(CleanuptGraphicResourcesEvent);

GraphicSystem::GraphicSystem(Kernel* kernel)
	: System(kernel, "Graphics")
{
	_kernel->GetEventManager()->AddHandler<WindowUpdateEvent>(this);
	_kernel->GetEventManager()->AddHandler<FrameBufferResizeEvent>(this);

	_renderingStageFactory.RegisterDefaultObjectGenerator
		<DeferredGeometryStage>("DeferredGeometryStage");
	_renderingStageFactory.RegisterDefaultObjectGenerator
		<DeferredLightingStage>("DeferredLightingStage");
}
GraphicSystem::~GraphicSystem()
{
	_kernel->GetEventManager()->RemoveHandler<WindowUpdateEvent>(this);
	_kernel->GetEventManager()->RemoveHandler<FrameBufferResizeEvent>(this);
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

bool GraphicSystem::Configure(const xml::Element* properties)
{
	if (!properties)
		return false;

	ConfigurePipelines(properties->FirstChildElement("RenderingPipelines"));
	ConfigureViewports(properties->FirstChildElement("Viewports"));

	_kernel->GetEventManager()->Broadcast(InitGraphicResourcesEvent{});
	return true;
}

bool GraphicSystem::Init()
{
	for (auto pipline : _pipelines)
		pipline->Init();
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

void GraphicSystem::HandleEvent(const FrameBufferResizeEvent& event)
{
	unsigned windowID = event.window->GetWindowID();
	if (_viewports.size() <= windowID)
		return;

	auto& windowViewports = _viewports[windowID];
	for (auto viewport : windowViewports)
	{
		viewport->UpdatePosition(event.size);
		viewport->UpdateSize(event.size);
	}
}

void GraphicSystem::ConfigurePipelines(const xml::Element* properties)
{
	if (!properties)
		return;

	for( auto pipelineProperties : xml::IterateChildElements(properties, "RenderingPipeline"))
	{
		auto pipline = std::make_shared<RenderingPipeline>(this);
		if (!pipline->Configure(pipelineProperties))
			pipline.reset();

		_pipelines.push_back(pipline);
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
