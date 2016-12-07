#ifndef BEMBEL_PERFORMMOVESTATE_H
#define BEMBEL_PERFORMMOVESTATE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "GameState.h"

#include "../chess/Moves/MoveSet.h"

#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class ChessBoard;
class ChessPiece;

class SelectChessPieceState;

}//end of namespace bembel

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class PerformMoveState : public GameState
{
public:
	PerformMoveState(StateMashine*, ChessBoard*);
	~PerformMoveState();

	using Move = MoveSet::Move;

	void Init(SelectChessPieceState*);

	virtual void OnEnterState() override;
	virtual void OnExitState() override;

	virtual void Update(double time) override;

	void SetMove(ChessPiece*, const Move&);
private:

	ChessBoard* _board;
	ChessPiece* _chessPiece;
	Move        _move;

	SelectChessPieceState* _selectChessPieceState = nullptr;

	double _time;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
