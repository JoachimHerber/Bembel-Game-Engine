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

#ifndef BEMBEL_PERFORMMOVESTATE_H
#define BEMBEL_PERFORMMOVESTATE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "game-state.h"

#include "../moves/move-set.h"

#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
class ChessPiece;
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class PerformMoveState : public GameState
{
public:
	PerformMoveState(ChessGame*);
	~PerformMoveState();

	using Move = MoveSet::Move;

	void Init( GameState*);

	virtual void OnEnterState() override;
	virtual void OnExitState() override;

	virtual void Update(double time) override;

	void SetMove(ChessPiece*, const Move&);

private:
	ChessPiece* _chessPiece;
	Move        _move;

	GameState* _nextState = nullptr;

	double _time;
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
