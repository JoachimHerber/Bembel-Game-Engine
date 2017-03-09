#ifndef BEMBEL_SELETCHESSPIECESTATE_H
#define BEMBEL_SELETCHESSPIECESTATE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "game-state.h"

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
class SelectionPointer;
class ChessPiece;
class SelectMoveState;
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class SelectChessPieceState : public GameState
{
public:
	SelectChessPieceState( ChessGame* game, unsigned player, SelectionPointer* );
	~SelectChessPieceState();

	void SetSelectMoveState( SelectMoveState* );

	virtual void OnEnterState();
	virtual void OnExitState();

	virtual void Update( double time );

	void SelectChessPiece();

private:
	void SetSelectedChessPice( ChessPiece* );

private:
	unsigned _player;

	SelectionPointer* _pointer;
	ChessPiece* _selectedChessPiece = nullptr;

	SelectMoveState* _selectMove;
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
