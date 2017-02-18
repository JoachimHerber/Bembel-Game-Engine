/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include "ChessApplication.h"
#include "SelectionRenderingStage.h"

#include "GameLogic/SelectChessBoardState.h"
#include "GameLogic/SelectChessPieceState.h"
#include "GameLogic/SelectMoveState.h"
#include "GameLogic/PerformMoveState.h"

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
	InitDefaultChessBoard();
	InitFourPlayerChessBoard();

	for (auto& it : _chessBoards)
		it->ResetChessBoard();

	InitGameStates();

	pipline->SetScene(_chessBoards[0]->GetScene());
	_cam->SetCameraOffset(glm::vec3(
		_chessBoards[0]->GetWidth(), 0.5f, _chessBoards[0]->GetHeight()));

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
	_stateMashine.Update(time);
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

bool ChessApplication::InitDefaultChessBoard()
{
	_chessBoards.push_back(
		std::make_unique<ChessBoard>( _kernel->GetAssetManager(), 8, 8));
	auto board = _chessBoards.back().get();

	board->GetPlayer(0)->SetMovementDirection(Player::RIGHT);
	board->GetPlayer(1)->SetMovementDirection(Player::LEFT);

	board->GetPlayer(0)->SetCaptureArea({-2, 0,  0}, {-2, 0, 0}, {0, 0, 2}, 8);
	board->GetPlayer(1)->SetCaptureArea({16, 0, 14}, {+2, 0, 0}, {0, 0,-2}, 8);

	board->AddChessPiece({0,0}, ChessBoard::ROOK, 0);
	board->AddChessPiece({0,1}, ChessBoard::KNIGHT, 0);
	board->AddChessPiece({0,2}, ChessBoard::BISHOP, 0);
	board->AddChessPiece({0,3}, ChessBoard::QUEEN, 0);
	board->AddChessPiece({0,4}, ChessBoard::KING, 0);
	board->AddChessPiece({0,5}, ChessBoard::BISHOP, 0);
	board->AddChessPiece({0,6}, ChessBoard::KNIGHT, 0);
	board->AddChessPiece({0,7}, ChessBoard::ROOK, 0);

	board->AddChessPiece({1,0}, ChessBoard::PAWN, 0);
	board->AddChessPiece({1,1}, ChessBoard::PAWN, 0);
	board->AddChessPiece({1,2}, ChessBoard::PAWN, 0);
	board->AddChessPiece({1,3}, ChessBoard::PAWN, 0);
	board->AddChessPiece({1,4}, ChessBoard::PAWN, 0);
	board->AddChessPiece({1,5}, ChessBoard::PAWN, 0);
	board->AddChessPiece({1,6}, ChessBoard::PAWN, 0);
	board->AddChessPiece({1,7}, ChessBoard::PAWN, 0);

	board->AddChessPiece({6,0}, ChessBoard::PAWN, 1);
	board->AddChessPiece({6,1}, ChessBoard::PAWN, 1);
	board->AddChessPiece({6,2}, ChessBoard::PAWN, 1);
	board->AddChessPiece({6,3}, ChessBoard::PAWN, 1);
	board->AddChessPiece({6,4}, ChessBoard::PAWN, 1);
	board->AddChessPiece({6,5}, ChessBoard::PAWN, 1);
	board->AddChessPiece({6,6}, ChessBoard::PAWN, 1);
	board->AddChessPiece({6,7}, ChessBoard::PAWN, 1);

	board->AddChessPiece({7,0}, ChessBoard::ROOK, 1);
	board->AddChessPiece({7,1}, ChessBoard::KNIGHT, 1);
	board->AddChessPiece({7,2}, ChessBoard::BISHOP, 1);
	board->AddChessPiece({7,3}, ChessBoard::QUEEN, 1);
	board->AddChessPiece({7,4}, ChessBoard::KING, 1);
	board->AddChessPiece({7,5}, ChessBoard::BISHOP, 1);
	board->AddChessPiece({7,6}, ChessBoard::KNIGHT, 1);
	board->AddChessPiece({7,7}, ChessBoard::ROOK, 1);

	return true;
}

bool ChessApplication::InitFourPlayerChessBoard()
{
	const std::vector<std::string> playerName = {"white","orange","black","blue"};
	_chessBoards.push_back(
		std::make_unique<ChessBoard>(
			_kernel->GetAssetManager(), 14, 14, playerName));
	auto board = _chessBoards.back().get();

	board->GetPlayer(0)->SetMovementDirection(Player::RIGHT);
	board->GetPlayer(1)->SetMovementDirection(Player::UP);
	board->GetPlayer(2)->SetMovementDirection(Player::LEFT);
	board->GetPlayer(3)->SetMovementDirection(Player::DOWN);

	board->GetPlayer(0)->SetCaptureArea({-2, 0,  6}, {-2, 0, 0}, { 0, 0, 2}, 8);
	board->GetPlayer(1)->SetCaptureArea({20, 0, -2}, { 0, 0,-2}, {-2, 0, 0}, 8);
	board->GetPlayer(2)->SetCaptureArea({28, 0, 20}, {+2, 0, 0}, { 0, 0,-2}, 8);
	board->GetPlayer(3)->SetCaptureArea({ 6, 0, 28}, { 0, 0,+2}, { 2, 0, 0}, 8);

	for (unsigned u = 0; u < 14; ++u)
	{
		if(2<u && u < 11)
			continue;

		for (unsigned v = 0; v < 14; ++v)
		{
			if (2<v && v < 11)
				continue;
			
			board->DisableTile(u, v);
		}
	}

	glm::ivec2 offsets[4]{{0,1},{-1,0},{0,-1},{1,0}};
	glm::ivec2 firstRow[4]{{0,3},{10,0},{13,10},{3,13}};
	glm::ivec2 secondRow[4]{{1,3},{10,1},{12,10},{3,12}};


	for (int i = 0; i < 4; ++i)
	{
		glm::ivec2 pos = firstRow[i];
		glm::ivec2 offset = offsets[i];

		board->AddChessPiece(pos+0*offset, ChessBoard::ROOK, i);
		board->AddChessPiece(pos+1*offset, ChessBoard::KNIGHT, i);
		board->AddChessPiece(pos+2*offset, ChessBoard::BISHOP, i);
		board->AddChessPiece(pos+3*offset, ChessBoard::QUEEN, i);
		board->AddChessPiece(pos+4*offset, ChessBoard::KING, i);
		board->AddChessPiece(pos+5*offset, ChessBoard::BISHOP, i);
		board->AddChessPiece(pos+6*offset, ChessBoard::KNIGHT, i);
		board->AddChessPiece(pos+7*offset, ChessBoard::ROOK, i);

		pos = secondRow[i];
		board->AddChessPiece(pos+0*offset, ChessBoard::PAWN, i);
		board->AddChessPiece(pos+1*offset, ChessBoard::PAWN, i);
		board->AddChessPiece(pos+2*offset, ChessBoard::PAWN, i);
		board->AddChessPiece(pos+3*offset, ChessBoard::PAWN, i);
		board->AddChessPiece(pos+4*offset, ChessBoard::PAWN, i);
		board->AddChessPiece(pos+5*offset, ChessBoard::PAWN, i);
		board->AddChessPiece(pos+6*offset, ChessBoard::PAWN, i);
		board->AddChessPiece(pos+7*offset, ChessBoard::PAWN, i);
	}
	return true;
}

bool ChessApplication::InitGameStates()
{
	auto keyboard = _interactionSys->GetKeyboard();
	_stateMashine.SetNextButton(keyboard->GetKey(GLFW_KEY_RIGHT));
	_stateMashine.SetPrevButton(keyboard->GetKey(GLFW_KEY_LEFT));
	_stateMashine.SetSelectButton(keyboard->GetKey(GLFW_KEY_SPACE));

	auto selectBoard = _stateMashine.CreateState<SelectChessBoardState>(
		_graphicSys->GetRenderingPiplies()[0].get(), _cam.get());

	for (auto& board : _chessBoards)
	{
		auto selectPiece =
			_stateMashine.CreateState<SelectChessPieceState>(board.get());
		auto selectMove =
			_stateMashine.CreateState<SelectMoveState>(board.get());
		auto performMove =
			_stateMashine.CreateState<PerformMoveState>(board.get());

		selectPiece->Init(selectMove);
		selectMove->Init(performMove);
		performMove->Init(selectPiece);

		selectBoard->AddChessBoard(board.get(), selectPiece);
	}
	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

