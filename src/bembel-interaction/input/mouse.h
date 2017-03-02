#ifndef BEMBEL_INPUTDEVICEMOUSE_H
#define BEMBEL_INPUTDEVICEMOUSE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include <BembelKernel/Events/EventManager.h>
#include <BembelKernel/Events/InputEvents.h>

#include "InputDevice.h"

#include <memory>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class BEMBEL_API Mouse : public InputDevice
{
public:
	Mouse(EventManager*);
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
