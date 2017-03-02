#ifndef BEMBEL_INPUTDEVICEKEYBOARD_H
#define BEMBEL_INPUTDEVICEKEYBOARD_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include "InputDevice.h"

#include <BembelKernel/Events/EventManager.h>
#include <BembelKernel/Events/InputEvents.h>

#include <memory>
#include <map>

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
