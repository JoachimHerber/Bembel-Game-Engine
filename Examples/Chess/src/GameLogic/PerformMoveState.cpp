/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "PerformMoveState.h"
#include "SelectChessPieceState.h"

#include "../chess/ChessPiece.h"
#include "../chess/ChessBoard.h"

#include <BembelKernel/Scene/Scene.h>
#include <BembelKernel/Scene/PositionComponent.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

PerformMoveState::PerformMoveState(StateMashine* sm, ChessBoard* board)
	: GameState(sm)
	, _board(board)
	, _chessPiece(nullptr)
	, _move({nullptr, 0})
{}

PerformMoveState::~PerformMoveState()
{}

void PerformMoveState::Init(SelectChessPieceState* state)
{
	_selectChessPieceState = state;
}

void PerformMoveState::OnEnterState()
{
	_move.move->StartMove(_chessPiece, _move.param);
	_time = 0;
}

void PerformMoveState::OnExitState()
{
	_move.move->EndeMove(_chessPiece, _move.param);

	unsigned player = _selectChessPieceState->GetCurrentPlayer();
	player = (player + 1)% _board->GetNumPlayer();
	_selectChessPieceState->SetCurrentPlayer(player);
}

void PerformMoveState::Update(double time)
{
	_time += time;
	if (_move.move->UpdateMoveAnimation(
		_time, _chessPiece, _move.param))
	{
		NextState(_selectChessPieceState);
	}
}

void PerformMoveState::SetMove(
	ChessPiece* chessPiece, const Move& move)
{
	_chessPiece = chessPiece;
	_move       = move;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
