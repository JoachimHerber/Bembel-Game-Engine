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

	virtual void OnNextButtonPress() = 0;
	virtual void OnPrevButtonPress() = 0;
	virtual void OnSelectButtonPress() = 0;

	virtual void OnEnterState() = 0;
	virtual void OnExitState()  = 0;

	virtual void Update(double time) = 0;

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
