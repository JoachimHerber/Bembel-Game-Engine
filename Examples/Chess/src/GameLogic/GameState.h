#ifndef BEMBEL_GAMESTATE_H
#define BEMBEL_GAMESTATE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/
namespace bembel{

class StateMashine;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class GameState
{
public:
	GameState(StateMashine* sm);
	virtual ~GameState();

	virtual void OnNextButtonPress(){};
	virtual void OnPrevButtonPress(){};
	virtual void OnSelectButtonPress(){};

	virtual void OnEnterState(){};
	virtual void OnExitState(){};

	virtual void Update(double time){};

protected:
	void NextState(GameState* state);

protected:
	StateMashine* _sm;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
