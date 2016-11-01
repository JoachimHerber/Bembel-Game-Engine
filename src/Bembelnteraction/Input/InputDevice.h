#ifndef BEMBEL_INPUTDEVICE_H
#define BEMBEL_INPUTDEVICE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>
#include "Button.h"

#include <string>
#include <map>

/*============================================================================*/
/* FORWARD DECLARATIONS                                                       */
/*============================================================================*/
namespace bembel{

class EventManager;

}//end of namespace bembel
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class BEMBEL_API InputDevice
{
protected:
	InputDevice(EventManager* eventMgr, const std::string& name);
	virtual ~InputDevice();

public:
	const std::string& GetDeviceName() const;

protected:
	EventManager*     _eventMgr;
	const std::string _name;
};

}//end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
