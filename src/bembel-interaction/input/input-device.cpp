/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "input-device.h"

/*============================================================================*/
/* IMPLEMENTATION			                                                  */
/*============================================================================*/
namespace bembel{

InputDevice::InputDevice(EventManager* eventMgr, const std::string& name)
	: _eventMgr(eventMgr)
	, _name(name)
{}

InputDevice::~InputDevice()
{}

const std::string& InputDevice::GetDeviceName() const
{
	return _name;
}

}//end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/

