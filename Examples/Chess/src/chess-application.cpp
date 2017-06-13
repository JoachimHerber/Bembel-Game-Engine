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
	_graphicSys     = kernel_->AddSystem<bembel::GraphicSystem>();
	_interactionSys = kernel_->AddSystem<bembel::InteractionSystem>();

	_graphicSys->GetRendertingStageFactory()
		.RegisterDefaultObjectGenerator<SelectionRenderingStage>(
			"SelectionRenderingStage");

	kernel_->GetEventManager()->AddHandler<bembel::WindowShouldCloseEvent>(this);
}

ChessApplication::~ChessApplication()
{}

bool ChessApplication::Init()
{
	if (!kernel_->LoadSetting("chess/config.xml"))
		return false;
	auto pipline = _graphicSys->GetRenderingPiplies()[0].get();

	_cam = std::make_shared<CameraControle>(
		kernel_->GetEventManager(), pipline->GetCamera());

	InitGame();
	_chessGame->ResetChessBoard();

	pipline->SetScene(_chessGame->GetScene());
	_cam->SetCameraOffset(glm::vec3( 8, 0.5f, 8));
	_cam->EnableManualControle(true);

	kernel_->InitSystems();
	return true;
}

void ChessApplication::Cleanup()
{
	kernel_->ShutdownSystems();
	kernel_->GetDisplayManager()->CloseOpenWindows();
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
		kernel_->GetAssetManager(),
		kernel_->GetEventManager(),
		_graphicSys );
	_chessGame->ResetChessBoard();

	return true;
}
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

