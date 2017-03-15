#ifndef BEMBEL_INTERACTION_INPUT_INPUTDEVICEMOUSE_H_
#define BEMBEL_INTERACTION_INPUT_INPUTDEVICEMOUSE_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"

#include "input-device.h"

#include <memory>

#include <bembel-kernel/events/event-manager.h>
#include <bembel-kernel/events/input-events.h>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class BEMBEL_API Mouse : public InputDevice
{
public:
	Mouse(EventManager*);
	Mouse( const Mouse& ) = delete;
	Mouse& operator= ( const Mouse& ) = delete;
	~Mouse();

	Button* GetButton(int);

	void HandleEvent(const MouseButtonPressEvent&);
	void HandleEvent(const MouseButtonReleaseEvent&);

private:
	// the use of unique_ptr with std::vector causes problems 
	// with Visual Studio so we use shared_ptr instead
	std::vector<std::shared_ptr<Button>> _buttons;
};

}//end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
