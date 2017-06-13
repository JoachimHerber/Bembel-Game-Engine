/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "perform-move.h"
#include "select-chess-piece.h"

#include "../chess-piece.h"
#include "../chess-game.h"

#include <bembel-kernel/scene/scene.h>
#include <bembel-kernel/scene/position-component.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
PerformMoveState::PerformMoveState( ChessGame* game)
	: GameState( game )
	, _chessPiece(nullptr)
	, _move({nullptr, 0})
{}

PerformMoveState::~PerformMoveState()
{}

void PerformMoveState::Init( GameState* state)
{
	_nextState = state;
}

void PerformMoveState::OnEnterState()
{
	_move.move->StartMove(_chessPiece, _game->GetChessBoard() ,_move.param);
	_time = 0;
}

void PerformMoveState::OnExitState()
{
	_move.move->EndeMove(_chessPiece, _game->GetChessBoard(), _move.param);
}

void PerformMoveState::Update(double time)
{
	_time += time;
	if (_move.move->UpdateMoveAnimation(
		_time, _chessPiece, _move.param))
	{
		NextState( _nextState );
	}
}

void PerformMoveState::SetMove(
	ChessPiece* chessPiece, const Move& move)
{
	_chessPiece = chessPiece;
	_move       = move;
}
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
