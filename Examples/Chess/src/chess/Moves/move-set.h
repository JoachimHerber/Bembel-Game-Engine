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

#ifndef BEMBEL_CHESSPIECEMOVESET_H
#define BEMBEL_CHESSPIECEMOVESET_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <glm/glm.hpp>

#include "../chess-game.h"

#include <memory>
#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
class ChessPiece;
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class MoveSet
{
public:
	class MoveTemplate
	{
	public:
		MoveTemplate(){};
		virtual ~MoveTemplate(){};

		virtual void GetPosibleMoveParameter(ChessPiece*, const ChessBoard&, std::vector<int>&) = 0;
		virtual glm::vec2 GetTargetPosition(ChessPiece*, int) = 0;

		virtual void StartMove(ChessPiece*, ChessBoard&, int) = 0;
		virtual void EndeMove(ChessPiece*, ChessBoard&, int) = 0;

		virtual bool UpdateMoveAnimation(double, ChessPiece*, int) = 0;
	};

	struct Move
	{
		MoveTemplate* move;
		int           param;
	};

public:
	MoveSet();
	~MoveSet();

	void AddMove(const glm::ivec2& dir, 
				 unsigned maxDist,
				 bool attack = true,
				 bool move = true);
	void AddMove(const glm::ivec2& dir,
				 bool attack = true,
				 bool move = true);

	void AddMoveTemplate(std::shared_ptr<MoveTemplate>);

	void GetAvailableMoves(ChessPiece*, const ChessBoard&, std::vector<Move>&);

private:
	std::vector<std::shared_ptr<MoveTemplate>> _moves;
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
