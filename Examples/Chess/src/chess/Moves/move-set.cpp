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
/* INCLUDES                                                                   */
/*============================================================================*/

#include "move-set.h"
#include "standard-move.h"

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
MoveSet::MoveSet()
{}

MoveSet::~MoveSet()
{}

void MoveSet::AddMove(
	const glm::ivec2& dir, 
	unsigned maxDist, 
	bool attack /*= true*/, 
	bool move /*= true*/)
{
	AddMoveTemplate(
		std::make_shared<StandardMove>(
			dir, maxDist, attack, move));
}

void MoveSet::AddMove(
	const glm::ivec2& dir,
	bool attack /*= true*/,
	bool move /*= true*/)
{
	AddMoveTemplate(
		std::make_shared<StandardMove>(
			dir, attack, move));
}

void MoveSet::AddMoveTemplate(
	std::shared_ptr<MoveTemplate> move)
{
	_moves.push_back(move);
}

void MoveSet::GetAvailableMoves(
	ChessPiece* chessPice, 
	const ChessBoard& board,
	std::vector<Move>& moves)
{
	for (auto& move : _moves)
	{
		std::vector<int> params;
		move->GetPosibleMoveParameter(
			chessPice, board, params);

		for (const auto& it : params)
		{
			moves.push_back(Move{move.get(), it});
		}
	}
}
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
