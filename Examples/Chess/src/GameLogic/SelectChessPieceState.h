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

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class SelectChessPieceState : public GameState
{
public:
	SelectChessPieceState(StateMashine*, Player*);
	~SelectChessPieceState();

	virtual void OnNextButtonPress() override;
	virtual void OnPrevButtonPress() override;
	virtual void OnSelectButtonPress() override;

	virtual void OnEnterState() override;
	virtual void OnExitState() override;

	virtual void Update(double time) override;

private:
	Player* _player;

	unsigned _selectedChessPiece;

	double _time = 0.0;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
