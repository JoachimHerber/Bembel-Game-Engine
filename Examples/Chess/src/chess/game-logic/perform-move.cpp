/*============================================================================*/
/* INCLUDES                                                                   */
/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2018 Joachim Herber                                     * */
/* *                                                                        * */
/* *   Permission is hereby granted, free of charge, to any person          * */
/* *   obtaining copy of this software and associated documentation files   * */
/* *   (the "Software"), to deal in the Software without restriction,       * */
/* *   including without limitation the rights to use, copy, modify, merge, * */
/* *   publish, distribute, sublicense, and/or sell copies of the Software, * */
/* *   and to permit persons to whom the Software is furnished to do so,    * */
/* *   subject to the following conditions :                                * */
/* *                                                                        * */
/* *   The above copyright notice and this permission notice shall be       * */
/* *   included in all copies or substantial portions of the Software.      * */
/* *                                                                        * */
/* *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      * */
/* *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   * */
/* *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                * */
/* *   NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   * */
/* *   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   * */
/* *   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    * */
/* *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     * */
/* *   SOFTWARE.                                                            * */
/* *                                                                        * */
/* ************************************************************************** */
/******************************************************************************/

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
