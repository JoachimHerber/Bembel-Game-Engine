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

class StandardMove : public ChessPieceMoveSet::MoveTemplate
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

	void GetAvailableTargetPositions(
		ChessPiece*, 
		ChessBoard*, 
		std::vector<glm::ivec2>&
	) override;

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
