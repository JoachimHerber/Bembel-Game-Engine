/*============================================================================*/
/* INLINE IMPLEMENTETION                                                      */
/*============================================================================*/
namespace bembel {

template <typename EventType>
template <typename EventHandlerType>
inline bool EventChannel<EventType>::AddHandler(EventHandlerType* handler)
{
	std::lock_guard<std::mutex> lock(mutex_);

	for (auto it : event_handler_)
	{
		if (it.event_handler == handler)
			return false; // handler already registered
	}

	HandlerAdapter adapter;
	adapter.event_handler = handler;
	adapter.forward_event_function = ForwardEvent<EventHandlerType>;
	event_handler_.push_back(adapter);
	return true;
}

template <typename EventType>
template <typename EventHandlerType>
inline bool EventChannel<EventType>::RemoveHandler(EventHandlerType* handler)
{
	std::lock_guard<std::mutex> lock(mutex_);

	for(auto it = event_handler_.begin(); it <= event_handler_.end(); ++it)
	{
		if(it->event_handler == handler)
		{
			event_handler_.erase(it);
			return true;
		}
	}

	return false; // handler not found
}

template <typename EventType>
inline void EventChannel<EventType>::Broadcast(EventType& event)
{
	std::vector<HandlerAdapter> handler;
	{
		std::lock_guard<std::mutex> lock(mutex_);
		handler = event_handler_;
	}

	for(auto it : handler)
		it.forward_event_function(it.event_handler, event);
}

template <typename EventType>
template <typename EventHandlerType>
inline void EventChannel<EventType>::ForwardEvent(
	void* handler, EventType& event)
{
	static_cast<EventHandlerType*>(handler)->HandleEvent(event);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/