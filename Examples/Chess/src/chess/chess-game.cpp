/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "chess-game.h"
#include "chess-piece.h"
#include "chess-piece-type.h"
#include "moves/initial-pawn-move.h"
#include "game-logic/select-chess-piece.h"
#include "game-logic/select-move.h"
#include "game-logic/perform-move.h"
#include "game-logic/selection-pointer.h"

#include "../selection-component.h"

#include <bembel-kernel/assets/asset-manager.h>
#include <bembel-kernel/scene/position-component.h>
#include <bembel-graphics/geometry/geometry-model.h>
#include <bembel-graphics/geometry/geometry-component.h>
#include <bembel-graphics/rendering-pipeline/light-source-component.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
ChessGame::ChessGame(
	bembel::AssetManager* assetMgr, 
	bembel::EventManager* eventMgr, 
	bembel::GraphicSystem* graphicSys )
	: _scene(std::make_shared<bembel::Scene>(assetMgr))
	, _selectionPointer( std::make_unique<SelectionPointer>( eventMgr, graphicSys, _scene.get() ) )
	, _player{Player{this,"white"}, Player{this,"black"}}
{
	_scene->RegisterComponentType<bembel::PositionComponent>();
	_scene->RegisterComponentType<bembel::DirLightProperties>();
	_scene->RegisterComponentType<bembel::GeometryComponent>();
	_scene->RegisterComponentType<SelectionComponent>();

	_scene->LoadAssets("assets/assets.xml");

	auto entity = _scene->CreateEntity();
	auto light = _scene->CreateComponent<bembel::DirLightProperties>(entity);
	light->direction = glm::normalize(glm::vec3(-0.3, -1, -0.2));
	light->color = 5.0f*glm::vec3(1, 1, 1);

	InitTiles();
	InitChessPieceTypes();
	InitChessPices();
	InitStates();
}

ChessGame::~ChessGame()
{}

std::shared_ptr<bembel::Scene> ChessGame::GetScene() const
{
	return _scene;
}

ChessBoard & ChessGame::GetChessBoard()
{
	return _board;
}

const ChessBoard & ChessGame::GetChessBoard() const
{
	return _board;
}


const std::array<Player, 2>& ChessGame::GetPlayer() const
{
	return _player;
}

void ChessGame::ResetChessBoard()
{
	_player[0].ClearChessPieces();
	_player[1].ClearChessPieces();
	_player[0].ClearCaptureChessPieces();
	_player[1].ClearCaptureChessPieces();

	for( auto& it1: _board )
	{
		for( auto& it2: it1 )
		{
			it2 = nullptr;
		}
	}

	for (auto& it : _chessPieces)
	{
		it->Reset();
		_player[it->GetOwner()].AddChessPiece(it.get());

		auto pos = it->GetPositon();
		_board[pos.x][pos.y] = it.get();
	}

}

void ChessGame::UpdatePossibleMoves()
{
	for (auto& it : _chessPieces)
		it->UpdatePossibleMoves(_board);
}

SelectionComponent* ChessGame::GetBoardTileSelectionComponent( const glm::ivec2& pos )
{
	if( pos.x < 0 || pos.x >= 8 || pos.y < 0 || pos.y >= 8 )
		return nullptr;

	return _scene->GetComponent<SelectionComponent>( _tiles[pos.x][pos.y]);
}

void ChessGame::Update( double time )
{
	if( _nextState != nullptr )
	{
		if( _currentState != nullptr )
			_currentState->OnExitState();

		_currentState = _nextState;
		_nextState = nullptr;

		_currentState->OnEnterState();
	}

	if( _currentState != nullptr )
	{
		_currentState->Update( time );
	}
}

void ChessGame::SetNextState( GameState* state )
{
	_nextState = state;
}

void ChessGame::InitTiles()
{
	auto whiteTile =
		_scene->GetAssetHandle<bembel::GeometryModel>("white.tile");
	auto blackTile =
		_scene->GetAssetHandle<bembel::GeometryModel>("black.tile");

	for (unsigned u = 0; u < 8; ++u)
	{
		for (unsigned v = 0; v < 8; ++v)
		{
			auto tile = _scene->CreateEntity();
			_tiles[u][v] = tile;

			auto posComp = _scene->CreateComponent<bembel::PositionComponent>(tile);
			posComp->position = glm::vec3(2.0f*u, 0, 2.0f*v);
			auto geomComt = _scene->CreateComponent<bembel::GeometryComponent>(tile);
			geomComt->model = ((u+v)%2 != 0 ? whiteTile : blackTile);
			auto selectComp = _scene->CreateComponent<SelectionComponent>(tile);
			selectComp->state = SelectionComponent::UNSELECTABLE;
		}
	}
}

void ChessGame::InitChessPieceTypes()
{
	_chessPieceTypes.resize(6);
	_chessPieceTypes[PAWN]   = CreateChessPieceType("pawn");
	_chessPieceTypes[ROOK]   = CreateChessPieceType("rook");
	_chessPieceTypes[KNIGHT] = CreateChessPieceType("knight");
	_chessPieceTypes[BISHOP] = CreateChessPieceType("bishop");
	_chessPieceTypes[QUEEN]  = CreateChessPieceType("queen");
	_chessPieceTypes[KING]   = CreateChessPieceType("king");

	_chessPieceTypes[PAWN]->AddMove({1,-1}, 1, true, false);
	_chessPieceTypes[PAWN]->AddMove({1,+0}, 1, false, true);
	_chessPieceTypes[PAWN]->AddMove({1,+1}, 1, true, false);
	_chessPieceTypes[PAWN]->GetMoveSet().AddMoveTemplate( 
		std::make_shared<InitialPawnMove>());

	_chessPieceTypes[ROOK]->AddMove({ 1, 0});
	_chessPieceTypes[ROOK]->AddMove({ 0,-1});
	_chessPieceTypes[ROOK]->AddMove({-1, 0});
	_chessPieceTypes[ROOK]->AddMove({ 0,+1});

	_chessPieceTypes[KNIGHT]->AddMove({+1,+2}, 1U);
	_chessPieceTypes[KNIGHT]->AddMove({+2,+1}, 1U);
	_chessPieceTypes[KNIGHT]->AddMove({+2,-1}, 1U);
	_chessPieceTypes[KNIGHT]->AddMove({+1,-2}, 1U);
	_chessPieceTypes[KNIGHT]->AddMove({-1,-2}, 1U);
	_chessPieceTypes[KNIGHT]->AddMove({-2,-1}, 1U);
	_chessPieceTypes[KNIGHT]->AddMove({-2,+1}, 1U);
	_chessPieceTypes[KNIGHT]->AddMove({-1,+2}, 1U);

	_chessPieceTypes[BISHOP]->AddMove({+1,+1});
	_chessPieceTypes[BISHOP]->AddMove({+1,-1});
	_chessPieceTypes[BISHOP]->AddMove({-1,-1});
	_chessPieceTypes[BISHOP]->AddMove({-1,+1});

	_chessPieceTypes[QUEEN]->AddMove({+1,+1});
	_chessPieceTypes[QUEEN]->AddMove({+1, 0});
	_chessPieceTypes[QUEEN]->AddMove({+1,-1});
	_chessPieceTypes[QUEEN]->AddMove({ 0,-1});
	_chessPieceTypes[QUEEN]->AddMove({-1,-1});
	_chessPieceTypes[QUEEN]->AddMove({-1, 0});
	_chessPieceTypes[QUEEN]->AddMove({-1,+1});
	_chessPieceTypes[QUEEN]->AddMove({ 0,+1});

	_chessPieceTypes[KING]->AddMove({+1,+1}, 1U);
	_chessPieceTypes[KING]->AddMove({+1, 0}, 1U);
	_chessPieceTypes[KING]->AddMove({+1,-1}, 1U);
	_chessPieceTypes[KING]->AddMove({ 0,-1}, 1U);
	_chessPieceTypes[KING]->AddMove({-1,-1}, 1U);
	_chessPieceTypes[KING]->AddMove({-1, 0}, 1U);
	_chessPieceTypes[KING]->AddMove({-1,+1}, 1U);
	_chessPieceTypes[KING]->AddMove({ 0,+1}, 1U);
}

void ChessGame::InitChessPices()
{
	AddChessPiece( {0,0}, ROOK, 0 );
	AddChessPiece( {0,1}, KNIGHT, 0 );
	AddChessPiece( {0,2}, BISHOP, 0 );
	AddChessPiece( {0,3}, QUEEN, 0 );
	AddChessPiece( {0,4}, KING, 0 );
	AddChessPiece( {0,5}, BISHOP, 0 );
	AddChessPiece( {0,6}, KNIGHT, 0 );
	AddChessPiece( {0,7}, ROOK, 0 );

	AddChessPiece( {1,0}, PAWN, 0 );
	AddChessPiece( {1,1}, PAWN, 0 );
	AddChessPiece( {1,2}, PAWN, 0 );
	AddChessPiece( {1,3}, PAWN, 0 );
	AddChessPiece( {1,4}, PAWN, 0 );
	AddChessPiece( {1,5}, PAWN, 0 );
	AddChessPiece( {1,6}, PAWN, 0 );
	AddChessPiece( {1,7}, PAWN, 0 );

	AddChessPiece( {6,0}, PAWN, 1 );
	AddChessPiece( {6,1}, PAWN, 1 );
	AddChessPiece( {6,2}, PAWN, 1 );
	AddChessPiece( {6,3}, PAWN, 1 );
	AddChessPiece( {6,4}, PAWN, 1 );
	AddChessPiece( {6,5}, PAWN, 1 );
	AddChessPiece( {6,6}, PAWN, 1 );
	AddChessPiece( {6,7}, PAWN, 1 );

	AddChessPiece( {7,0}, ROOK, 1 );
	AddChessPiece( {7,1}, KNIGHT, 1 );
	AddChessPiece( {7,2}, BISHOP, 1 );
	AddChessPiece( {7,3}, QUEEN, 1 );
	AddChessPiece( {7,4}, KING, 1 );
	AddChessPiece( {7,5}, BISHOP, 1 );
	AddChessPiece( {7,6}, KNIGHT, 1 );
	AddChessPiece( {7,7}, ROOK, 1 );
}

void ChessGame::InitStates()
{
	auto selectChessPiece0 = std::make_unique<SelectChessPieceState>(this, 0, _selectionPointer.get());
	auto selectChessPiece1 = std::make_unique<SelectChessPieceState>(this, 1, _selectionPointer.get());
	auto selectMove0       = std::make_unique<SelectMoveState>(this, _selectionPointer.get());
	auto selectMove1       = std::make_unique<SelectMoveState>(this, _selectionPointer.get());
	auto performMove0      = std::make_unique<PerformMoveState>(this);
	auto performMove1      = std::make_unique<PerformMoveState>(this);
	

	selectChessPiece0->SetSelectMoveState( selectMove0.get() );
	selectChessPiece1->SetSelectMoveState( selectMove1.get() );
	selectMove0->SetPerformMoveState( performMove0.get() );
	selectMove1->SetPerformMoveState( performMove1.get() );
	performMove0->Init( selectChessPiece1.get());
	performMove1->Init( selectChessPiece0.get() );

	_states.push_back( std::move( selectChessPiece0 ) );
	_states.push_back( std::move( selectChessPiece1 ) );
	_states.push_back( std::move( selectMove0 ) );
	_states.push_back( std::move( selectMove1 ) );
	_states.push_back( std::move( performMove0 ) );
	_states.push_back( std::move( performMove1 ) );


	_nextState = _states[0].get();
}

void ChessGame::AddChessPiece(
	const glm::vec2& pos,
	unsigned type,
	unsigned owner )
{
	_chessPieces.push_back( std::make_unique<ChessPiece>(
		_chessPieceTypes[type].get(), _scene.get(), owner, pos ));
}

std::unique_ptr<ChessPieceType> ChessGame::CreateChessPieceType(
	const std::string& modleSufix)
{
	auto type = std::make_unique<ChessPieceType>();

	type->GetModles()[0] = _scene->GetAssetHandle<bembel::GeometryModel>(
		_player[0].GetName() + "." + modleSufix );
	type->GetModles()[1] = _scene->GetAssetHandle<bembel::GeometryModel>(
		_player[1].GetName() + "." + modleSufix );

	return std::move(type);
}
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
