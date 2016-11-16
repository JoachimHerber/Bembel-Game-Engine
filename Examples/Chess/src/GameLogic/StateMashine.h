#ifndef BEMBEL_STATEMANAGER_H
#define BEMBEL_STATEMANAGER_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <memory>
#include <vector>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class GameState;
class Button;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class StateMashine final
{
public:
	StateMashine();
	~StateMashine();

	template<class TState, typename ... TArgs>
	TState* CreateState(TArgs&& ... args);

	void SetNextButton(Button*);
	void SetPrevButton(Button*);
	void SetSelectButton(Button*);

	void SetNextState(GameState* state);

	void Update(double time);

	void NextButtonPress();
	void PrevButtonPress();
	void SelectButtonPress();

private:
	std::vector<std::unique_ptr<GameState>> _states;

	GameState* _currentState = nullptr;
	GameState* _nextState    = nullptr;

	Button* _nextButton     = nullptr;
	Button* _prevButton     = nullptr;
	Button* _selectedButton = nullptr;
};

}//end of namespace bembel
 /*============================================================================*/
 /* TEMPLATE IMPLEMENTATION                                                    */
 /*============================================================================*/
namespace bembel{

template<class TState, typename ... TArgs>
inline TState* StateMashine::CreateState(TArgs&& ... args)
{
	auto state = new TState(this, std::forward<TArgs>(args)...);
	_states.push_back(std::unique_ptr<GameState>(state));
	if (_currentState == nullptr && _nextState == nullptr)
		_nextState = state;

	return state;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
