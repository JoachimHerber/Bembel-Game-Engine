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

bool System::Init()
{
	_eventMgr->Broadcast(
		SystemInitializationEvent{this});
	return true;
}

bool System::Init(std::string& configFileName)
{
	xml::Document doc;
	if (doc.LoadFile(configFileName.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		BEMBEL_LOG_ERROR()
			<< "Failed to lode file '" << configFileName << "'\n"
			<< doc.ErrorName() << std::endl;
		return false;
	}
	return Init(doc.FirstChildElement(_name.c_str()));
}

bool System::Init(const xml::Element* properties)
{
	return Init();
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
