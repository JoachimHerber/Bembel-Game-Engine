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

#ifndef BEMBEL_CHESSPIECETYPE_H
#define BEMBEL_CHESSPIECETYPE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <bembel-kernel/assets/asset-handle.h>
#include <bembel-kernel/assets/asset-manager.h>

#include "moves/move-set.h"

#include <memory>
#include <array>
#include <map>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class ChessPieceType
{
public:
	ChessPieceType();
	~ChessPieceType();

	std::array<bembel::AssetHandle, 2>& GetModles();
	const std::array<bembel::AssetHandle, 2>& GetModles() const;

	void AddMove(const glm::ivec2& dir,
				 unsigned maxDist,
				 bool attack = true,
				 bool move = true);
	void AddMove(const glm::ivec2& dir,
				 bool attack = true,
				 bool move = true);
	MoveSet& GetMoveSet();

private:
	std::array<bembel::AssetHandle, 2> _modles;
	
	MoveSet _moveSet;
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
