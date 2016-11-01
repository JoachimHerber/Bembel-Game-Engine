#ifndef BEMBEL_BUTTON_H
#define BEMBEL_BUTTON_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include "../Signal.hpp"

#include <BembelKernel/Events/EventManager.h>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API Button
{
public:
	Button(const std::string& name);
	~Button();

	const std::string& GetName() const;

	bool GetIsPressed() const;
	void SetIsPressed(bool);

	Signal<>& GetPressSignal();
	Signal<>& GetReleaseSignal();

protected:
	std::string _name;

	Signal<> _pressSignal;
	Signal<> _releaseSignal;

	bool _pressed;
};

struct BEMBEL_API ButtonPressEvent
{
	// the button that was pressed
	Button* button;

	BEMBEL_EVENT_INTERVACE_DECLADRATION
};

struct BEMBEL_API ButtonReleaseEvent
{
	// the button that has been released
	Button* button;

	BEMBEL_EVENT_INTERVACE_DECLADRATION
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
