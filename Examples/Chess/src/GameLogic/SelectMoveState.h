#ifndef BEMBEL_SELECTMOVESTATE_H
#define BEMBEL_SELECTMOVESTATE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "GameState.h"

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class ChessPiece;
class ChessBoard;
class PerformMoveState;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class SelectMoveState : public GameState
{
public:
	SelectMoveState(StateMashine*, ChessBoard*);
	~SelectMoveState();

	void Init(PerformMoveState*);

	virtual void OnNextButtonPress() override;
	virtual void OnPrevButtonPress() override;
	virtual void OnSelectButtonPress() override;

	virtual void OnEnterState() override;
	virtual void OnExitState() override;

	void SetChessPiece(ChessPiece*);

	void OnMoveChanged();

private:
	ChessBoard* _board;
	ChessPiece* _chessPiece;
	unsigned _selectedMove;

	PerformMoveState* _performMove = nullptr;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
