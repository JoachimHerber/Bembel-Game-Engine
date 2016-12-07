#ifndef BEMBEL_INITIALPAWNMOVE_H
#define BEMBEL_INITIALPAWNMOVE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "MoveSet.h"

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class InitialPawnMove : public MoveSet::MoveTemplate
{
public:
	InitialPawnMove();
	~InitialPawnMove();

	virtual void GetPosibleMoveParameter(ChessPiece*, std::vector<int>&) override;

	virtual glm::vec2 GetTargetPosition(ChessPiece*, int) override;

	virtual void StartMove(ChessPiece*, int) override;
	virtual void EndeMove(ChessPiece*, int) override;

	virtual bool UpdateMoveAnimation(double, ChessPiece*, int) override;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
