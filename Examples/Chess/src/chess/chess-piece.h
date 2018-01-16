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

#ifndef BEMBEL_CHESSPIECE_H
#define BEMBEL_CHESSPIECE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <bembel-kernel/scene/scene.h>

#include "moves/move-set.h"
#include "chess-game.h"

#include <glm/glm.hpp>
#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
class ChessPieceType;
namespace bembel{

class Scene;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class ChessPiece
{
public:
	ChessPiece(ChessPieceType*, bembel::Scene*, unsigned,  const glm::ivec2&);

	void Promote(ChessPieceType*);

	ChessPieceType* GetType() const;
	bembel::Scene*  GetScene() const;
	unsigned        GetOwner() const;

	const glm::ivec2& GetPositon() const;
	void SetPosition(const glm::ivec2& pos);

	bembel::Scene::EntityID GetEntity();

	bool IsAlive() const;
	void Kill();
	void Reset();

	bool HasMoved() const;

	void UpdatePossibleMoves(const ChessBoard&);

	const std::vector<MoveSet::Move>& GetPossibleMoves() const;
private:
	bembel::Scene* scene_;

	ChessPieceType* _type;
	ChessPieceType* _originalType;
	unsigned        _owner;
	glm::ivec2      _position;
	glm::ivec2      _startPositon;

	bool            _isAlive;
	bool			_hasMoved;

	bembel::Scene::EntityID _entity;

	std::vector<MoveSet::Move> _possibleMoves;
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
