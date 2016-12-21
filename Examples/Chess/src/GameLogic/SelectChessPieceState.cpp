/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "SelectChessPieceState.h"
#include "SelectMoveState.h"

#include "../SelectionComponent.h"
#include "../chess/Player.h"
#include "../chess/ChessPiece.h"
#include "../chess/ChessBoard.h"

#include <BembelKernel/Scene/Scene.h>

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
			break;
		}
	}

	OnChessPieceChanged();
}

void SelectChessPieceState::OnExitState()
{
	auto scene  = _board->GetScene();
	auto player = _board->GetPlayer(_player);
	auto piece = player->GetChessPieces()[_selectedChessPiece]->GetEntity();

	_selectMove->SetChessPiece(player->GetChessPieces()[_selectedChessPiece]);
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
	auto scene      = _board->GetScene();
	auto player     = _board->GetPlayer(_player);
	auto  container = scene->RequestComponentContainer<SelectionComponent>();
	auto& selectionComponents = container->GetComponents();

	for (auto& it : selectionComponents)
		it.state = SelectionComponent::UNSELECTABLE;

	auto piece = player->GetChessPieces()[_selectedChessPiece];

	selectionComponents[piece->GetEntity()].state = 
		SelectionComponent::FOCUSED;

	for (auto it : piece->GetPossibleMoves())
	{
		auto target = it.move->GetTargetPosition(piece, it.param);

		auto tile = _board->GetTileEntity(target);
		selectionComponents[tile].state =
			SelectionComponent::SELECTABLE;
	}

}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
