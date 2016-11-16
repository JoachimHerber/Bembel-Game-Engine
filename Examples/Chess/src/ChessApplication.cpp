/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
#include "ChessApplication.h"

#include "GameLogic/SelectChessPieceState.h"
#include "GameLogic/SelectChessBoardState.h"

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
	auto pipline = _graphicSys->GetRenderingPiplies()[0];

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
	_assetMgr = std::make_shared<AssetManager>();
	_assetMgr->RegisterAssetType<Material>();
	_assetMgr->RegisterAssetType<GeometryMesh>();
	_assetMgr->RegisterAssetType<GeometryModel>();
	_assetMgr->LoadAssets("assets/assets.xml");

	return true;
}

bool ChessApplication::InitDefaultChessBoard()
{
	_chessBoards.push_back(
		std::make_unique<ChessBoard>(_assetMgr, 8, 8));

	_chessBoards.back()->AddChessPiece({0,0}, ChessBoard::ROOK, 0);
	_chessBoards.back()->AddChessPiece({0,1}, ChessBoard::KNIGHT, 0);
	_chessBoards.back()->AddChessPiece({0,2}, ChessBoard::BISHOP, 0);
	_chessBoards.back()->AddChessPiece({0,3}, ChessBoard::QUEEN, 0);
	_chessBoards.back()->AddChessPiece({0,4}, ChessBoard::KING, 0);
	_chessBoards.back()->AddChessPiece({0,5}, ChessBoard::BISHOP, 0);
	_chessBoards.back()->AddChessPiece({0,6}, ChessBoard::KNIGHT, 0);
	_chessBoards.back()->AddChessPiece({0,7}, ChessBoard::ROOK, 0);

	_chessBoards.back()->AddChessPiece({1,0}, ChessBoard::PAWN, 0);
	_chessBoards.back()->AddChessPiece({1,1}, ChessBoard::PAWN, 0);
	_chessBoards.back()->AddChessPiece({1,2}, ChessBoard::PAWN, 0);
	_chessBoards.back()->AddChessPiece({1,3}, ChessBoard::PAWN, 0);
	_chessBoards.back()->AddChessPiece({1,4}, ChessBoard::PAWN, 0);
	_chessBoards.back()->AddChessPiece({1,5}, ChessBoard::PAWN, 0);
	_chessBoards.back()->AddChessPiece({1,6}, ChessBoard::PAWN, 0);
	_chessBoards.back()->AddChessPiece({1,7}, ChessBoard::PAWN, 0);

	_chessBoards.back()->AddChessPiece({6,0}, ChessBoard::PAWN, 1);
	_chessBoards.back()->AddChessPiece({6,1}, ChessBoard::PAWN, 1);
	_chessBoards.back()->AddChessPiece({6,2}, ChessBoard::PAWN, 1);
	_chessBoards.back()->AddChessPiece({6,3}, ChessBoard::PAWN, 1);
	_chessBoards.back()->AddChessPiece({6,4}, ChessBoard::PAWN, 1);
	_chessBoards.back()->AddChessPiece({6,5}, ChessBoard::PAWN, 1);
	_chessBoards.back()->AddChessPiece({6,6}, ChessBoard::PAWN, 1);
	_chessBoards.back()->AddChessPiece({6,7}, ChessBoard::PAWN, 1);

	_chessBoards.back()->AddChessPiece({7,0}, ChessBoard::ROOK, 1);
	_chessBoards.back()->AddChessPiece({7,1}, ChessBoard::KNIGHT, 1);
	_chessBoards.back()->AddChessPiece({7,2}, ChessBoard::BISHOP, 1);
	_chessBoards.back()->AddChessPiece({7,3}, ChessBoard::QUEEN, 1);
	_chessBoards.back()->AddChessPiece({7,4}, ChessBoard::KING, 1);
	_chessBoards.back()->AddChessPiece({7,5}, ChessBoard::BISHOP, 1);
	_chessBoards.back()->AddChessPiece({7,6}, ChessBoard::KNIGHT, 1);
	_chessBoards.back()->AddChessPiece({7,7}, ChessBoard::ROOK, 1);

	return true;
}

bool ChessApplication::InitFourPlayerChessBoard()
{
	const std::vector<std::string> playerName = {"white","orange","black","blue"};
	_chessBoards.push_back(
		std::make_unique<ChessBoard>(
			_assetMgr, 14, 14, playerName));

	for (unsigned u = 0; u < 14; ++u)
	{
		if(2<u && u < 11)
			continue;

		for (unsigned v = 0; v < 14; ++v)
		{
			if (2<v && v < 11)
				continue;
			
			_chessBoards.back()->DisableTile(u, v);
		}
	}

	glm::ivec2 offsets[4]{{0,1},{-1,0},{0,-1},{1,0}};
	glm::ivec2 firstRow[4]{{0,3},{10,0},{13,10},{3,13}};
	glm::ivec2 secondRow[4]{{1,3},{10,1},{12,10},{3,12}};


	for (int i = 0; i < 4; ++i)
	{
		glm::ivec2 pos = firstRow[i];
		glm::ivec2 offset = offsets[i];

		_chessBoards.back()->AddChessPiece(pos+0*offset, ChessBoard::ROOK, i);
		_chessBoards.back()->AddChessPiece(pos+1*offset, ChessBoard::KNIGHT, i);
		_chessBoards.back()->AddChessPiece(pos+2*offset, ChessBoard::BISHOP, i);
		_chessBoards.back()->AddChessPiece(pos+3*offset, ChessBoard::QUEEN, i);
		_chessBoards.back()->AddChessPiece(pos+4*offset, ChessBoard::KING, i);
		_chessBoards.back()->AddChessPiece(pos+5*offset, ChessBoard::BISHOP, i);
		_chessBoards.back()->AddChessPiece(pos+6*offset, ChessBoard::KNIGHT, i);
		_chessBoards.back()->AddChessPiece(pos+7*offset, ChessBoard::ROOK, i);

		pos = secondRow[i];
		_chessBoards.back()->AddChessPiece(pos+0*offset, ChessBoard::PAWN, i);
		_chessBoards.back()->AddChessPiece(pos+1*offset, ChessBoard::PAWN, i);
		_chessBoards.back()->AddChessPiece(pos+2*offset, ChessBoard::PAWN, i);
		_chessBoards.back()->AddChessPiece(pos+3*offset, ChessBoard::PAWN, i);
		_chessBoards.back()->AddChessPiece(pos+4*offset, ChessBoard::PAWN, i);
		_chessBoards.back()->AddChessPiece(pos+5*offset, ChessBoard::PAWN, i);
		_chessBoards.back()->AddChessPiece(pos+6*offset, ChessBoard::PAWN, i);
		_chessBoards.back()->AddChessPiece(pos+7*offset, ChessBoard::PAWN, i);
	}
	return true;
}

bool ChessApplication::InitGameStates()
{
	auto keyboard = _interactionSys->GetKeyboard();
	_stateMashine.SetNextButton(keyboard->GetKey(GLFW_KEY_RIGHT));
	_stateMashine.SetPrevButton(keyboard->GetKey(GLFW_KEY_LEFT));
	_stateMashine.SetSelectButton(keyboard->GetKey(GLFW_KEY_SPACE));

	auto boardSelect = _stateMashine.CreateState<SelectChessBoardState>(
		_graphicSys->GetRenderingPiplies()[0].get(), _cam.get());

	for (auto& board : _chessBoards)
	{
		std::vector<GameState*> selectPlayerStages;
		for (size_t n = 0; n < board->GetNumPlayer(); ++n)
		{
			selectPlayerStages.push_back(
				_stateMashine.CreateState<SelectChessPieceState>(
					board->GetPlayer(n)));
		}
		boardSelect->AddChessBoard(board.get(), selectPlayerStages[0]);
	}
	return true;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

