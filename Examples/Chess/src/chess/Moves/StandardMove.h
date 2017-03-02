#ifndef BEMBEL_STANDARDMOVE_H
#define BEMBEL_STANDARDMOVE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "MoveSet.h"

#include <glm/glm.hpp>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class StandardMove : public MoveSet::MoveTemplate
{
public:
	StandardMove(
		const glm::ivec2& dir, 
		unsigned maxDist,
		bool attack = true,
		bool move = true);
	StandardMove(
		const glm::ivec2& dir,
		bool attack = true,
		bool move = true);
	~StandardMove();

	virtual void GetPosibleMoveParameter(
		ChessPiece*, 
		const ChessBoard&,
		std::vector<int>&)
		override;
	virtual glm::vec2 GetTargetPosition(ChessPiece*, int)
		override;

	virtual void StartMove(ChessPiece*, ChessBoard&, int) override;
	virtual void EndeMove(ChessPiece*, ChessBoard&, int) override;

	virtual bool UpdateMoveAnimation(double, ChessPiece*, int) 
		override;
private:
	glm::ivec2 _direction;
	unsigned   _maxDistance;
	bool       _attack;
	bool       _move;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
