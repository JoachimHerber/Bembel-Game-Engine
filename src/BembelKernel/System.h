#ifndef BEMBE_SYSTEM_H
#define BEMBE_SYSTEM_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <BembelConfig.h>

#include "Events/EventManager.h"

#include <BembelBase/XML.h>

#include <memory>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

class BEMBEL_API System
{
public:
	System(std::shared_ptr<EventManager> eventMgr, const std::string& name);
	virtual ~System();
	
	const std::string& GetName() const;
	EventManager* GetEventManager() const;

	virtual bool Configure(const xml::Element*);

	virtual bool Init();
	virtual void Shutdown();

	virtual void Update(double timeSinceLastUpdate) = 0;

protected:
	std::shared_ptr<EventManager> _eventMgr;
	const std::string             _name;
};

struct BEMBEL_API SystemInitializationEvent
{
	System* system;

	BEMBEL_EVENT_INTERVACE_DECLADRATION

};

struct BEMBEL_API SystemShutdownEvent
{
	System* system;

	BEMBEL_EVENT_INTERVACE_DECLADRATION
};

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
