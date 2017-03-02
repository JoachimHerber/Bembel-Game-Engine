#ifndef BEMBEL_KERNEL_EVENTS_CHANNEL_H_
#define BEMBEL_KERNEL_EVENTS_CHANNEL_H_
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <mutex>
#include <vector>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

class EventChannelBase
{
public:
	EventChannelBase(){};
	virtual ~EventChannelBase(){};
};

template <typename EventType>
class EventChannel : public EventChannelBase
{
public:
	EventChannel(){};
	~EventChannel(){};

	template <typename EventHandlerType>
	bool AddHandler(EventHandlerType* handler);
	template <typename EventHandlerType>
	bool RemoveHandler(EventHandlerType* handler);

	void Broadcast(EventType& event);

private:
	template <typename EventHandlerType>
	static void ForwardEvent(void*, EventType&);
	using ForwardEventFunction = void(*)(void*, EventType&);

	struct HandlerAdapter
	{	
		void*                eventHandler;
		ForwardEventFunction forwardEvent;
	};

	std::vector<HandlerAdapter> _handler;

	std::mutex _mutex;
};

} //end of namespace bembel
/*============================================================================*/
/* INLINE IMPLEMENTATIONS													  */
/*============================================================================*/

#include "event-channel.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
