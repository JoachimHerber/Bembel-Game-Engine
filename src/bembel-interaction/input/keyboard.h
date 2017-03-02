#ifndef BEMBEL_INTERACTION_INPUT_INPUTDEVICEKEYBOARD_H_
#define BEMBEL_INTERACTION_INPUT_INPUTDEVICEKEYBOARD_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "input-device.h"

#include <memory>
#include <map>

#include <bembel-kernel/events/event-manager.h>
#include <bembel-kernel/events/input-events.h>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class BEMBEL_API Keyboard : public InputDevice
{
public:
	Keyboard(EventManager*);
	~Keyboard();

	Button* GetKey(int keyID, int scancode = -1);

	void HandleEvent(const KeyPressEvent&);
	void HandleEvent(const KeyReleaseEvent&);

private:
	// the use of unique_ptr with std::vector causes problems 
	// with Visual Studio so we use shared_ptr instead
	std::vector<std::shared_ptr<Button>>   _keys;

	std::map<std::pair<int, int>, unsigned> _keyMapping;
};


}//end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
