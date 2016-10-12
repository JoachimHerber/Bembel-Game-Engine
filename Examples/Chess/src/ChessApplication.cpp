/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include "ChessApplication.h"

#include <BembelOpenGL.h>

#include <BembelKernel/Kernel.h>
#include <BembelKernel/Display/DisplayManager.h>
#include <BembelKernel/Scene/PositionComponent.h>
#include <BembelKernel/Assets/SerialAssetLoader.hpp>
#include <BembelKernel/Renderig/Material.h>
#include <BembelKernel/Renderig/GeometryMesh.h>
#include <BembelKernel/Renderig/GeometryModel.h>

#include <BembelGraphics/Geometry/GeometryInstance.h>
#include <BembelGraphics/RenderingPipeline/LightSourceProperties.h>

#include <chrono>
#include <random>
#include <iostream>

#include <GLFW/glfw3.h>
#include "BembelGraphics/Geometry/GeometryRenderer.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

ChessApplication::ChessApplication()
	: bembel::Application()
{
	_graphicSys = std::make_shared<bembel::GraphicSystem>(_kernel.get());
	_kernel->AddSystem(_graphicSys);
	_kernel->GetEventManager()->AddHandler<bembel::WindowShouldCloseEvent>(this);

	auto& rendererFactory = _graphicSys->GetRendererFactory();
	rendererFactory.RegisterDefaultObjectGenerator
		<bembel::GeometryRenderer>("GeomRenderer");
}

ChessApplication::~ChessApplication()
{}


bool ChessApplication::Init()
{
	if (!_kernel->LoadSetting("config.xml"))
		return false;
	auto pipline = _graphicSys->GetRenderingPiplies()[0];

	_cam = std::make_shared<CameraControle>(
		_kernel->GetEventManager(), pipline->GetCamera());

	InitAssets();
	InitScene();

	pipline->SetScene(_scene);

	_kernel->InitSystems();
	return true;
}

void ChessApplication::Cleanup()
{
	_kernel->ShutdownSystems();
	_kernel->GetDisplayManager()->CloseOpenWindows();
}

void ChessApplication::Update(double time)
{
	_cam->Update(time);
}

void ChessApplication::HandleEvent(const WindowShouldCloseEvent& event)
{
	Quit();
}

bool ChessApplication::InitAssets()
{
	_assetMgr = std::make_shared<AssetManager>();
	_assetMgr->RegisterAssetType<Material>();
	_assetMgr->RegisterAssetType<GeometryMesh>();
	_assetMgr->RegisterAssetType<GeometryModel>();
	_assetMgr->LoadAssets("assets/assets.xml");

	return true;
}

bool ChessApplication::InitScene()
{
	_scene = std::make_shared<Scene>(_assetMgr);

	_scene->RegisterComponentType<PositionComponent>();
	_scene->RegisterComponentType<PointLightProperties>();
	_scene->RegisterComponentType<DirLightProperties>();
	_scene->RegisterComponentType<GeometryInstance>();

	auto entity = _scene->CreateEntity();
	auto light = _scene->CreateComponent<DirLightProperties>(entity);
	light->direction = glm::normalize(glm::vec3(-0.3, -1, -0.2));
	light->color = 3.0f*glm::vec3(1, 1, 1);
	

	_board = std::make_shared<ChessBoard>();

	_board->Init(_scene, 8, 8);

	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

