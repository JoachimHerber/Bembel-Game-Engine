#ifndef BEMBEL_SELETCHESSPIECESTATE_H
#define BEMBEL_SELETCHESSPIECESTATE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "GameState.h"

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class Player;
class ChessBoard;
class SelectMoveState;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class SelectChessPieceState : public GameState
{
public:
	SelectChessPieceState(StateMashine*, ChessBoard*);
	~SelectChessPieceState();

	void Init(SelectMoveState*);

	virtual void OnNextButtonPress() override;
	virtual void OnPrevButtonPress() override;
	virtual void OnSelectButtonPress() override;

	virtual void OnEnterState() override;
	virtual void OnExitState() override;

	virtual void Update(double time) override;

	void     SetCurrentPlayer(unsigned);
	unsigned GetCurrentPlayer() const;

	void OnChessPieceChanged();

private:
	ChessBoard* _board;
	unsigned    _player;

	unsigned _selectedChessPiece;
	double _time = 0.0;

	SelectMoveState* _selectMove;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
