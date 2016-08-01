/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include "RenderingTest.h"

#include "SimpleGeometryRenderer.h"
#include "SimpleGeometryComponent.h"

#include <BembelOpenGL.h>

#include <BembelKernel/Kernel.h>
#include <BembelKernel/Display/DisplayManager.h>
#include <BembelKernel/Scene/PositionComponent.h>
#include <BembelKernel/Assets/SerialAssetLoader.hpp>

#include <BembelGraphics/Viewport.h>
#include <BembelGraphics/TextureView.h>
#include <BembelGraphics/Geometry/Material.h>
#include <BembelGraphics/RenderingPipeline/Camera.h>
#include <BembelGraphics/RenderingPipeline/RenderingPipeline.h>
#include <BembelGraphics/RenderingPipeline/DeferredGeometryStage.h>
#include <BembelGraphics/RenderingPipeline/DeferredLightingStage.h>
#include <BembelGraphics/RenderingPipeline/LightSourceProperties.h>

#include <chrono>
#include <random>
#include <iostream>

#include <GLFW/glfw3.h>


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
	_kernel->GetEventManager()->AddHandler<bembel::KeyPressEvent>(this);
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

	InitScene();

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

void RenderingTest::NextScene()
{
	_currentScene = (_currentScene +1)%_scenes.size();

	_graphicSys->GetRenderingPiplies()[0]->SetScene(_scenes[_currentScene]);
}

void RenderingTest::PrevScene()
{
	_currentScene = (_currentScene +_scenes.size() - 1)%_scenes.size();
	_graphicSys->GetRenderingPiplies()[0]->SetScene(_scenes[_currentScene]);
}

void RenderingTest::HandleEvent(const WindowShouldCloseEvent& event)
{
	Quit();
}

void RenderingTest::HandleEvent(const KeyPressEvent& event)
{
	if (event.keyID == GLFW_KEY_RIGHT)
		NextScene();
	if (event.keyID == GLFW_KEY_LEFT)
		NextScene();
}

void RenderingTest::InitScene()
{
	_assetMgr = std::make_shared<AssetManager>();
	CreateSerialAssetLoader<Texture>(_assetMgr.get());
	CreateSerialAssetLoader<Material>(_assetMgr.get());
	
	_scenes.push_back(LoadScene("chessboard.xml"));
	_scenes.push_back(LoadScene("planets.xml"));

	_currentScene = 0;
	_graphicSys->GetRenderingPiplies()[0]->SetScene(_scenes[_currentScene]);
}

std::shared_ptr<Scene> RenderingTest::LoadScene(const std::string& file)
{
	auto scene = std::make_shared<Scene>(_assetMgr);

	scene->RegisterComponentType<PositionComponent>();
	scene->RegisterComponentType<PointLightProperties>();
	scene->RegisterComponentType<DirLightProperties>();
	scene->RegisterComponentType<SimpleGeometryComponent>();

	scene->LoadScene(file);
	return scene;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

