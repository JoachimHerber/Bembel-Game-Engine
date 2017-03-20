#ifndef BEMBEL_KERNEL_EVENTS_EVENTMANAGER_H_
#define BEMBEL_KERNEL_EVENTS_EVENTMANAGER_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "bembel-config.h"
#include "event-channel.hpp"

#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class BEMBEL_API EventManager final
{
public:
	EventManager();
	EventManager(const EventManager&) = delete;
	EventManager& operator=(const EventManager&) = delete;
	~EventManager();

	template <typename EventType, typename EventHandlerType>
	bool AddHandler(EventHandlerType* handler);
	template <typename EventType, typename EventHandlerType>
	bool RemoveHandler(EventHandlerType* handler);

	template <typename EventType>
	void Broadcast(EventType& event);

	template <typename EventType>
	EventChannel<EventType>* GetChannel();

private:
	using EventChannelPointer = std::unique_ptr<EventChannelBase>;
	std::unordered_map<std::type_index, EventChannelPointer> channels_;
};

} //end of namespace bembel
/*============================================================================*/
/* INLINE IMPLEMENTATIONS													  */
/*============================================================================*/

#include "event-manager.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
