#ifndef BEMBEL_CHESSPIECE_H
#define BEMBEL_CHESSPIECE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelKernel/Scene/Scene.h>

#include "Moves/MoveSet.h"

#include <glm/glm.hpp>
#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class Player;
class ChessPieceType;
class Scene;
class ChessBoard;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class ChessPiece
{
public:
	ChessPiece(ChessPieceType*, Player*, const glm::ivec2&);

	void Promote(ChessPieceType*);

	ChessPieceType* GetType() const;
	Player*         GetOwner() const;
	ChessBoard*     GetBoard() const;

	const glm::ivec2& GetPositon() const;
	void SetPosition(const glm::ivec2& pos);

	Scene::EntityID GetEntity();

	bool IsAlive() const;
	void Kill(Player*);
	void Reset();

	bool HasMoved() const;

	void UpdatePossibleMoves();

	const std::vector<MoveSet::Move>& GetPossibleMoves() const;
private:
	Scene* _scene;

	ChessPieceType* _type;
	ChessPieceType* _originalType;
	Player*         _owner;
	glm::ivec2      _position;
	glm::ivec2      _startPositon;

	bool            _isAlive;
	bool			_hasMoved;

	Scene::EntityID _entity;

	std::vector<MoveSet::Move> _possibleMoves;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
