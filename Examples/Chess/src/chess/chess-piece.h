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
