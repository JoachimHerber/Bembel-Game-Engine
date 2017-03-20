#ifndef BEMBEL_INTERACTION_INPUT_BUTTON_H_
#define BEMBEL_INTERACTION_INPUT_BUTTON_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include "../signal.hpp"

#include <bembel-kernel/events/event-manager.h>

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
};

struct BEMBEL_API ButtonReleaseEvent
{
	// the button that has been released
	Button* button;
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
