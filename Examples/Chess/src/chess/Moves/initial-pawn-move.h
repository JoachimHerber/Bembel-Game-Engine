#ifndef BEMBEL_INITIALPAWNMOVE_H
#define BEMBEL_INITIALPAWNMOVE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "move-set.h"

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class InitialPawnMove : public MoveSet::MoveTemplate
{
public:
	InitialPawnMove();
	~InitialPawnMove();

	virtual void GetPosibleMoveParameter(ChessPiece*, const ChessBoard&, std::vector<int>&) override;

	virtual glm::vec2 GetTargetPosition(ChessPiece*, int) override;

	virtual void StartMove(ChessPiece*, ChessBoard&, int) override;
	virtual void EndeMove(ChessPiece*, ChessBoard&, int) override;

	virtual bool UpdateMoveAnimation(double, ChessPiece*, int) override;
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
