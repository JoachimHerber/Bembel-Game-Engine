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

#ifndef BEMBEL_SELETCHESSPIECESTATE_H
#define BEMBEL_SELETCHESSPIECESTATE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "game-state.h"

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
class SelectionPointer;
class ChessPiece;
class SelectMoveState;
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class SelectChessPieceState : public GameState
{
public:
	SelectChessPieceState( ChessGame* game, unsigned player, SelectionPointer* );
	~SelectChessPieceState();

	void SetSelectMoveState( SelectMoveState* );

	virtual void OnEnterState();
	virtual void OnExitState();

	virtual void Update( double time );

	void SelectChessPiece();

private:
	void SetSelectedChessPice( ChessPiece* );

private:
	unsigned _player;

	SelectionPointer* _pointer;
	ChessPiece* _selectedChessPiece = nullptr;

	SelectMoveState* _selectMove;
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
