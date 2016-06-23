/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include "RenderingTest.h"

#include "SimpleGeometryRenderer.h"
#include "SimpleGeometryComponent.h"

#include <BembelOpenGL.h>

#include <BembelKernel/Kernel.h>
#include <BembelKernel/Engine/Engine.h>
#include <BembelKernel/Display/DisplayManager.h>

#include <BembelDataStructures/PositionComponent.h>

#include <BembelGraphics/Viewport.h>
#include <BembelGraphics/TextureView.h>
#include <BembelGraphics/RenderingPipeline/Camera.h>
#include <BembelGraphics/RenderingPipeline/RenderingPipeline.h>
#include <BembelGraphics/RenderingPipeline/DeferredGeometryStage.h>
#include <BembelGraphics/RenderingPipeline/DeferredLightingStage.h>
#include <BembelGraphics/RenderingPipeline/LightSourceProperties.h>

#include <chrono>
#include <random>
#include <iostream>


/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

RenderingTest::RenderingTest()
	: bembel::Application()
{
	_graphicSys = std::make_shared<bembel::GraphicSystem>(_kernel->GetEventManager());
	_kernel->GetEngine()->AddSystem(_graphicSys);
	_kernel->GetEventManager()->AddHandler<bembel::WindowShouldCloseEvent>(this);
	_kernel->GetEventManager()->AddHandler<bembel::FrameBufferResizeEvent>(this);
	_graphicSys->GetRendererFactory().RegisterDefaultObjectGenerator<SimpleGeometryRenderer>("TestRenderer");
}

RenderingTest::~RenderingTest()
{}


bool RenderingTest::Init()
{
	if (!_kernel->LoadSetting("config.xml"))
		return false;
	auto pipline = _graphicSys->GetRenderingPiplies()[0];

	_cam = std::make_shared<CameraControle>(
		_kernel->GetEventManager(), pipline->GetCamera());

	auto world = std::make_shared<bembel::EntityManager>();
	pipline->SetEntityManager(world);

	world->LoadEntities("scene.xml");
	return true;
}

void RenderingTest::Cleanup()
{
	_kernel->GetEngine()->ShutdownSystems();
	_kernel->GetDisplayManager()->CloseOpenWindows();
}

void RenderingTest::Update(double time)
{
	_cam->Update(time);
}

void RenderingTest::HandleEvent(const bembel::WindowShouldCloseEvent& event)
{
	Quit();
}

void RenderingTest::HandleEvent(const bembel::FrameBufferResizeEvent& event)
{
	int w = event.size.x/2;
	int h = event.size.y/2;

	_graphicSys->GetViewports()[0]->SetPosition(glm::ivec2(0, 0));
	_graphicSys->GetViewports()[1]->SetPosition(glm::ivec2(w, 0));
	_graphicSys->GetViewports()[2]->SetPosition(glm::ivec2(0, h));
	_graphicSys->GetViewports()[3]->SetPosition(glm::ivec2(w, h));
	_graphicSys->GetViewports()[0]->SetSize(glm::ivec2(w, h));
	_graphicSys->GetViewports()[1]->SetSize(glm::ivec2(w, h));
	_graphicSys->GetViewports()[2]->SetSize(glm::ivec2(w, h));
	_graphicSys->GetViewports()[3]->SetSize(glm::ivec2(w, h));

	_graphicSys->GetRenderingPiplies()[0]->SetResulution(event.size);
	_graphicSys->GetRenderingPiplies()[0]->GetCamera()->SetUpProjection(
		2, float(event.size.x)/float(event.size.y), 0.01f, 100.0f);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

