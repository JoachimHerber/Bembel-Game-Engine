/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "StateMashine.h"
#include "GameState.h"

#include <Bembelnteraction/Input/Button.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel {

StateMashine::StateMashine()
{}

StateMashine::~StateMashine()
{}

void StateMashine::SetNextState(GameState* state)
{
	_nextState = state;
}

void StateMashine::Update(double time)
{
	if (_nextState != nullptr)
	{
		if (_currentState != nullptr)
			_currentState->OnExitState();

		_currentState = _nextState;
		_nextState    = nullptr;

		_currentState->OnEnterState();
	}

	if (_currentState != nullptr)
	{
		_currentState->Update(time);
	}
}

void StateMashine::SetNextButton(Button* button)
{
	if (_nextButton != nullptr)
	{
		_nextButton->GetPressSignal().RemoveSlot(
			this, &StateMashine::NextButtonPress);
	}
	_nextButton = button;
	if (_nextButton != nullptr)
	{
		_nextButton->GetPressSignal().AddSlot(
			this, &StateMashine::NextButtonPress);
	}
}

void StateMashine::SetPrevButton(Button* button)
{
	if (_prevButton != nullptr)
	{
		_prevButton->GetPressSignal().RemoveSlot(
			this, &StateMashine::PrevButtonPress);
	}
	_prevButton = button;
	if (_prevButton != nullptr)
	{
		_prevButton->GetPressSignal().AddSlot(
			this, &StateMashine::PrevButtonPress);
	}
}

void StateMashine::SetSelectButton(Button* button)
{
	if (_selectedButton != nullptr)
	{
		_selectedButton->GetPressSignal().RemoveSlot(
			this, &StateMashine::SelectButtonPress);
	}
	_selectedButton = button;
	if (_selectedButton != nullptr)
	{
		_selectedButton->GetPressSignal().AddSlot(
			this, &StateMashine::SelectButtonPress);
	}
}

void StateMashine::NextButtonPress()
{
	if (_currentState != nullptr)
		_currentState->OnNextButtonPress();
}

void StateMashine::PrevButtonPress()
{
	if (_currentState != nullptr)
		_currentState->OnPrevButtonPress();
}

void StateMashine::SelectButtonPress()
{
	if (_currentState != nullptr)
		_currentState->OnSelectButtonPress();
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

