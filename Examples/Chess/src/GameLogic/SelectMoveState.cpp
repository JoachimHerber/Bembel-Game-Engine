/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "SelectMoveState.h"
#include "PerformMoveState.h"
#include "../SelectionComponent.h"
#include "../chess/Player.h"
#include "../chess/ChessPiece.h"
#include "../chess/ChessBoard.h"

#include <BembelKernel/Scene/Scene.h>
#include <BembelKernel/Scene/PositionComponent.h>

#include <Bembelnteraction/Input/Button.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

SelectMoveState::SelectMoveState(
	StateMashine* sm, 
	ChessBoard* board)
	: GameState(sm)
	, _board(board)
{}

SelectMoveState::~SelectMoveState()
{}

void SelectMoveState::Init(PerformMoveState* state)
{
	_performMove = state;
}

void SelectMoveState::OnNextButtonPress()
{
	_selectedMove += 1;
	_selectedMove %= _chessPiece->GetPossibleMoves().size();

	OnMoveChanged();
}

void SelectMoveState::OnPrevButtonPress()
{
	_selectedMove += _chessPiece->GetPossibleMoves().size() -1;
	_selectedMove %= _chessPiece->GetPossibleMoves().size();
	
	OnMoveChanged();
}

void SelectMoveState::OnSelectButtonPress()
{
	NextState(_performMove);
}

void SelectMoveState::OnEnterState()
{
	_selectedMove = 0;
	OnMoveChanged();
}

void SelectMoveState::OnExitState()
{
	_performMove->SetMove(
		_chessPiece,
		_chessPiece->GetPossibleMoves()[_selectedMove]
	);
}

void SelectMoveState::SetChessPiece(ChessPiece* pice)
{
	_chessPiece = pice;
}

void SelectMoveState::OnMoveChanged()
{
	auto scene = _board->GetScene();
	auto  container = scene->RequestComponentContainer<SelectionComponent>();
	auto& selectionComponents = container->GetComponents();

	for (auto& it : selectionComponents)
		it.stat = SelectionComponent::UNSELECTABLE;

	selectionComponents[_chessPiece->GetEntity()].stat =
		SelectionComponent::SELECED;

	for (size_t n = 0; n< _chessPiece->GetPossibleMoves().size(); ++n)
	{
		auto& move = _chessPiece->GetPossibleMoves()[n];
		auto target = move.move->GetTargetPosition(_chessPiece, move.param);

		auto tile = _board->GetTileEntity(target);
		selectionComponents[tile].stat =
			n == _selectedMove ?
			SelectionComponent::FOCUSED :
			SelectionComponent::SELECTABLE;
	}
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
