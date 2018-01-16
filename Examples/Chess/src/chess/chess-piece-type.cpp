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

#include "chess-piece-type.h"

#include <bembel-graphics/geometry/geometry-model.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
ChessPieceType::ChessPieceType()
{}

ChessPieceType::~ChessPieceType()
{}

std::array<bembel::AssetHandle, 2>& ChessPieceType::GetModles()
{
	return _modles;
}

const std::array<bembel::AssetHandle, 2>& ChessPieceType::GetModles() const
{
	return _modles;
}

void ChessPieceType::AddMove(const glm::ivec2& dir, unsigned maxDist, bool attack /*= true*/, bool move /*= true*/)
{
	_moveSet.AddMove(dir, maxDist, attack, move);
}

void ChessPieceType::AddMove(const glm::ivec2& dir, bool attack /*= true*/, bool move /*= true*/)
{
	_moveSet.AddMove(dir, attack, move);
}

MoveSet& ChessPieceType::GetMoveSet()
{
	return _moveSet;
}
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
