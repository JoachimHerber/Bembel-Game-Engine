/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include "chess-application.h"
#include "selection-rendering-stage.h"

#include <bembel-kernel/kernel.h>
#include <bembel-kernel/display/display-manager.h>
#include <bembel-graphics/rendering-pipeline/rendering-pipeline.h>
#include <bembel-interaction/input/keyboard.h>

#include <chrono>
#include <random>
#include <iostream>

#include <GLFW/glfw3.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
ChessApplication::ChessApplication()
	: bembel::Application()
{
	_graphicSys =
		std::make_shared<bembel::GraphicSystem>(_kernel.get());
	_interactionSys = 
		std::make_shared<bembel::InteractionSystem>(_kernel.get());

	_graphicSys->GetRendertingStageFactory()
		.RegisterDefaultObjectGenerator<SelectionRenderingStage>(
			"SelectionRenderingStage");

	_kernel->AddSystem(_graphicSys);
	_kernel->AddSystem(_interactionSys);
	_kernel->GetEventManager()->AddHandler<bembel::WindowShouldCloseEvent>(this);
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

void ChessApplication::HandleEvent(const bembel::WindowShouldCloseEvent& event)
{
	Quit();
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
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

