/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "SelectChessPieceState.h"
#include "SelectMoveState.h"
#include "../chess/Player.h"
#include "../chess/ChessPiece.h"
#include "../chess/ChessBoard.h"

#include <BembelKernel/Scene/Scene.h>
#include <BembelKernel/Scene/PositionComponent.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

SelectChessPieceState::SelectChessPieceState(StateMashine* sm, ChessBoard* board)
	: GameState(sm)
	, _board(board)
	, _player(0)
{}

SelectChessPieceState::~SelectChessPieceState()
{}

void SelectChessPieceState::Init(SelectMoveState* state)
{
	_selectMove = state;
}

void SelectChessPieceState::OnNextButtonPress()
{
	auto& pieces = _board->GetPlayer(_player)->GetChessPieces();
	for (size_t n = 1; n < pieces.size(); ++n)
	{
		size_t piece = (_selectedChessPiece + n)%pieces.size();
		if (!pieces[piece]->GetPossibleMoves().empty())
		{
			_selectedChessPiece = piece;
			break;
		}
	}
	OnChessPieceChanged();
}

void SelectChessPieceState::OnPrevButtonPress()
{
	auto& pieces = _board->GetPlayer(_player)->GetChessPieces();
	for (size_t n = 1; n < pieces.size(); ++n)
	{
		size_t piece = (_selectedChessPiece + pieces.size() - n)%pieces.size();
		if (!pieces[piece]->GetPossibleMoves().empty())
		{
			_selectedChessPiece = piece;
			break;
		}
	}
	OnChessPieceChanged();
}

void SelectChessPieceState::OnSelectButtonPress()
{
	NextState(_selectMove);
}

void SelectChessPieceState::OnEnterState()
{
	_board->UpdatePossibleMoves();
	_selectedChessPiece = 0;
	auto& pieces = _board->GetPlayer(_player)->GetChessPieces();
	for (size_t n = 0; n < pieces.size(); ++n)
	{
		if (!pieces[n]->GetPossibleMoves().empty())
		{
			_selectedChessPiece = n;
			return;
		}
	}
}

void SelectChessPieceState::OnExitState()
{
	auto scene  = _board->GetScene();
	auto player = _board->GetPlayer(_player);
	auto piece = player->GetChessPieces()[_selectedChessPiece]->GetEntity();
	auto pos = scene->GetComponent<PositionComponent>(piece);

	_selectMove->SetChessPiece(player->GetChessPieces()[_selectedChessPiece]);
	pos->position.y = 0.0f;
}

void SelectChessPieceState::Update(double time)
{
	_time += time*2*3.14159265359;

	auto scene  = _board->GetScene();
	auto player = _board->GetPlayer(_player);
	auto piece = player->GetChessPieces()[_selectedChessPiece];
	auto  posContainer = scene->RequestComponentContainer<PositionComponent>();
	auto& positions = posContainer->GetComponents();

	positions[piece->GetEntity()].position.y =
		0.3f + 0.3f*std::sin(_time);

	for (const auto& move : piece->GetPossibleMoves())
	{
		glm::ivec2 target = move.move->GetTargetPosition(piece, move.param);
		auto tile = _board->GetTileEntity(target);

		if(tile != Scene::INVALID_ENTITY)
			positions[tile].position.y =
			  0.1f + 0.05f*std::sin(
				  0.7836*_time +
				  1.678*target.x + 
				  3.764*target.y);
	}
}

void SelectChessPieceState::SetCurrentPlayer(unsigned id)
{
	_player = id;
}

unsigned SelectChessPieceState::GetCurrentPlayer() const
{
	return _player;
}

void SelectChessPieceState::OnChessPieceChanged()
{
	auto scene         = _board->GetScene();
	auto player        = _board->GetPlayer(_player);
	auto  posContainer = scene->RequestComponentContainer<PositionComponent>();
	auto& positions    = posContainer->GetComponents();

	for (auto& it : player->GetChessPieces())
		positions[it->GetEntity()].position.y = 0;

	for (int u = 0; u < _board->GetWidth(); ++u)
	{
		for (int v = 0; v < _board->GetHeight(); ++v)
		{
			if (!_board->IsPositionValid({u,v}))
				continue;

			positions[_board->GetTileEntity({u,v})].position.y = 0;
		}
	}
	_time = 0.0f;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
