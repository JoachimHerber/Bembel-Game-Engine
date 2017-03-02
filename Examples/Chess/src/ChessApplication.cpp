/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include "ChessApplication.h"
#include "SelectionRenderingStage.h"

#include "chess/Player.h"
#include "chess/ChessPiece.h"

#include <BembelOpenGL.h>

#include <BembelKernel/Kernel.h>
#include <BembelKernel/Display/DisplayManager.h>
#include <BembelKernel/Scene/PositionComponent.h>
#include <BembelKernel/Assets/SerialAssetLoader.hpp>

#include <BembelGraphics/Geometry/Material.h>
#include <BembelGraphics/Geometry/GeometryMesh.h>
#include <BembelGraphics/Geometry/GeometryModel.h>
#include <BembelGraphics/Geometry/GeometryComponent.h>

#include <BembelGraphics/RenderingPipeline/LightSourceProperties.h>

#include <Bembelnteraction/Input/Keyboard.h>

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
	_graphicSys =
		std::make_shared<GraphicSystem>(_kernel.get());
	_interactionSys = 
		std::make_shared<InteractionSystem>(_kernel.get());

	_graphicSys->GetRendertingStageFactory()
		.RegisterDefaultObjectGenerator<SelectionRenderingStage>(
			"SelectionRenderingStage");

	_kernel->AddSystem(_graphicSys);
	_kernel->AddSystem(_interactionSys);
	_kernel->GetEventManager()->AddHandler<WindowShouldCloseEvent>(this);
}

ChessApplication::~ChessApplication()
{}

bool ChessApplication::Init()
{
	if (!_kernel->LoadSetting("config.xml"))
		return false;
	auto pipline = _graphicSys->GetRenderingPiplies()[0].get();

	_cam = std::make_shared<CameraControle>(
		_kernel->GetEventManager(), pipline->GetCamera());

	InitAssets();
	InitGame();
	_chessGame->ResetChessBoard();

	pipline->SetScene(_chessGame->GetScene());
	_cam->SetCameraOffset(glm::vec3( 8, 0.5f, 8));
	_cam->EnableManualControle(true);

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
	_chessGame->Update(time);
}

void ChessApplication::HandleEvent(const WindowShouldCloseEvent& event)
{
	Quit();
}

bool ChessApplication::InitAssets()
{
	_kernel->GetAssetManager()->LoadAssets("assets/assets.xml");
	return true;
}

bool ChessApplication::InitGame()
{
	_chessGame = std::make_unique<ChessGame>( 
		_kernel->GetAssetManager(),
		_kernel->GetEventManager(),
		_graphicSys.get() );
	_chessGame->ResetChessBoard();

	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

