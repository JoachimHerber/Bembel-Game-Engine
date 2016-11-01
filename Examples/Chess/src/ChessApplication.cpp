/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include "ChessApplication.h"

#include <BembelOpenGL.h>

#include <BembelKernel/Kernel.h>
#include <BembelKernel/Display/DisplayManager.h>
#include <BembelKernel/Scene/PositionComponent.h>
#include <BembelKernel/Scene/GeometryComponent.h>
#include <BembelKernel/Assets/SerialAssetLoader.hpp>
#include <BembelKernel/Rendering/Material.h>
#include <BembelKernel/Rendering/GeometryMesh.h>
#include <BembelKernel/Rendering/GeometryModel.h>

#include <BembelGraphics/RenderingPipeline/LightSourceProperties.h>

#include <chrono>
#include <random>
#include <iostream>

#include <GLFW/glfw3.h>

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
	_scene->RegisterComponentType<GeometryComponent>();

	auto entity = _scene->CreateEntity();
	auto light = _scene->CreateComponent<DirLightProperties>(entity);
	light->direction = glm::normalize(glm::vec3(-0.3, -1, -0.2));
	light->color = 3.0f*glm::vec3(1, 1, 1);
	

	_board = std::make_shared<ChessBoard>();

	_board->Init(_scene, 8, 8);
	_board->AddChessPiece({0,0}, "white.rook");
	_board->AddChessPiece({1,0}, "white.knight");
	_board->AddChessPiece({2,0}, "white.bishop");
	_board->AddChessPiece({3,0}, "white.king");
	_board->AddChessPiece({4,0}, "white.queen");
	_board->AddChessPiece({5,0}, "white.bishop");
	_board->AddChessPiece({6,0}, "white.knight");
	_board->AddChessPiece({7,0}, "white.rook");
	_board->AddChessPiece({0,1}, "white.pawn");
	_board->AddChessPiece({1,1}, "white.pawn");
	_board->AddChessPiece({2,1}, "white.pawn");
	_board->AddChessPiece({3,1}, "white.pawn");
	_board->AddChessPiece({4,1}, "white.pawn");
	_board->AddChessPiece({5,1}, "white.pawn");
	_board->AddChessPiece({6,1}, "white.pawn");
	_board->AddChessPiece({7,1}, "white.pawn");

	_board->AddChessPiece({0,7}, "black.rook");
	_board->AddChessPiece({1,7}, "black.knight");
	_board->AddChessPiece({2,7}, "black.bishop");
	_board->AddChessPiece({3,7}, "black.king");
	_board->AddChessPiece({4,7}, "black.queen");
	_board->AddChessPiece({5,7}, "black.bishop");
	_board->AddChessPiece({6,7}, "black.knight");
	_board->AddChessPiece({7,7}, "black.rook");
	_board->AddChessPiece({0,6}, "black.pawn");
	_board->AddChessPiece({1,6}, "black.pawn");
	_board->AddChessPiece({2,6}, "black.pawn");
	_board->AddChessPiece({3,6}, "black.pawn");
	_board->AddChessPiece({4,6}, "black.pawn");
	_board->AddChessPiece({5,6}, "black.pawn");
	_board->AddChessPiece({6,6}, "black.pawn");
	_board->AddChessPiece({7,6}, "black.pawn");

	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

