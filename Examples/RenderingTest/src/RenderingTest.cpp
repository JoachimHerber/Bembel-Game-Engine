/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include "RenderingTest.h"

#include "SimpleGeometryRenderer.h"
#include "SimpleGeometryComponent.h"

#include <BembelOpenGL.h>

#include <BembelKernel/Kernel.h>
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
	_graphicSys = std::make_shared<bembel::GraphicSystem>(_kernel.get());
	_kernel->AddSystem(_graphicSys);
	_kernel->GetEventManager()->AddHandler<bembel::WindowShouldCloseEvent>(this);
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

	_kernel->InitSystems();

	return true;
}

void RenderingTest::Cleanup()
{
	_kernel->ShutdownSystems();
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

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

