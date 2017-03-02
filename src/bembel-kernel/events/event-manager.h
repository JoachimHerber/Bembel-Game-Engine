#ifndef BEMBEL_KERNEL_EVENTS_EVENTMANAGER_H_
#define BEMBEL_KERNEL_EVENTS_EVENTMANAGER_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "event-channel.hpp"

#include <vector>
#include <memory>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API EventManager final
{
public:
	EventManager();
	~EventManager();

	template <typename EventType, typename EventHandlerType>
	bool AddHandler(EventHandlerType* handler);
	template <typename EventType, typename EventHandlerType>
	bool RemoveHandler(EventHandlerType* handler);

	template <typename EventType>
	void Broadcast(EventType& event);

	template <typename EventType>
	EventChannel<EventType>* GetChannel();

	static unsigned GetNextFreeEventTypeID();

private:
	std::vector<EventChannelBase*> _channels;
	
	static unsigned _nextUnusedEventTypeID;
};

using EventManagerPtr = std::shared_ptr<EventManager>;

#define BEMBEL_EVENT_INTERVACE_DECLADRATION \
public:\
	static unsigned GetEventTypeID();\
private:\
	static unsigned _eventTypeID;

#define BEMBEL_EVENT_INTERVACE_IMPLEMENTATION( _CLASS ) \
unsigned _CLASS::_eventTypeID  = EventManager::GetNextFreeEventTypeID();\
unsigned _CLASS::GetEventTypeID(){return _eventTypeID;}

} //end of namespace bembel
/*============================================================================*/
/* INLINE IMPLEMENTATIONS													  */
/*============================================================================*/

#include "event-manager.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
