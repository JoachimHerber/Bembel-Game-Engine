#ifndef BEMBEL_PERFORMMOVESTATE_H
#define BEMBEL_PERFORMMOVESTATE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "game-state.h"

#include "../moves/move-set.h"

#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
class ChessPiece;
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
class PerformMoveState : public GameState
{
public:
	PerformMoveState(ChessGame*);
	~PerformMoveState();

	using Move = MoveSet::Move;

	void Init( GameState*);

	virtual void OnEnterState() override;
	virtual void OnExitState() override;

	virtual void Update(double time) override;

	void SetMove(ChessPiece*, const Move&);

private:
	ChessPiece* _chessPiece;
	Move        _move;

	GameState* _nextState = nullptr;

	double _time;
};
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
