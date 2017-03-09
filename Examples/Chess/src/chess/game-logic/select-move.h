#ifndef BEMBEL_SELECTMOVESTATE_H
#define BEMBEL_SELECTMOVESTATE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "game-state.h"

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/

class SelectionPointer;
class ChessPiece;
class PerformMoveState;
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class SelectMoveState : public GameState
{
public:
	SelectMoveState( ChessGame* game, SelectionPointer* );
	~SelectMoveState();

	void SetPerformMoveState( PerformMoveState* );

	void SetChessPiece( ChessPiece* );

	virtual void OnEnterState();
	virtual void OnExitState();

	virtual void Update( double time );

	void SelectedMove();

private:
	void UpdateSelection(int i);
private:
	SelectionPointer* _pointer;
	ChessPiece* _chessPiece = nullptr;

	int _selctedMove = -1;

	PerformMoveState* _performMove;
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
