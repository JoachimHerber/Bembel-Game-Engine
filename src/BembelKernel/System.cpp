/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "System.h"
#include <BembelBase/Logging/Logger.h>

/*============================================================================*/
/* IMPLEMENTATION        													  */
/*============================================================================*/
namespace bembel{

BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(SystemInitializationEvent)
BEMBEL_EVENT_INTERVACE_IMPLEMENTATION(SystemShutdownEvent)

System::System(std::shared_ptr<EventManager> eventMgr, const std::string& name)
	: _eventMgr(eventMgr)
	, _name(name)
{}

System::~System()
{}

const std::string& System::GetName() const
{
	return _name;
}
EventManager* System::GetEventManager() const
{
	return _eventMgr.get();
}

bool System::Configure(const xml::Element*)
{
	return true;
}

bool System::Init()
{
	_eventMgr->Broadcast(
		SystemInitializationEvent{this});
	return true;
}

void System::Shutdown()
{
	_eventMgr->Broadcast(
		SystemShutdownEvent{this});
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
