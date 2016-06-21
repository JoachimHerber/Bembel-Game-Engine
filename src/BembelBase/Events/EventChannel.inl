/*============================================================================*/
/* INLINE IMPLEMENTETION                                                      */
/*============================================================================*/
namespace bembel {

template <typename EventType>
template <typename EventHandlerType>
inline bool EventChannel<EventType>::AddHandler(EventHandlerType* handler)
{
	std::lock_guard<std::mutex> lock(_mutex);

	for (auto it : _handler)
	{
		if (it.eventHandler == handler)
			return false; // handler already registered
	}

	HandlerAdapter adapter;
	adapter.eventHandler = handler;
	adapter.forwardEvent = ForwardEvent<EventHandlerType>;
	_handler.push_back(adapter);
	return true;
}

template <typename EventType>
template <typename EventHandlerType>
inline bool EventChannel<EventType>::RemoveHandler(EventHandlerType* handler)
{
	std::lock_guard<std::mutex> lock(_mutex);

	for(auto it = _handler.begin(); it <= _handler.end(); ++it)
	{
		if(it->eventHandler == handler)
		{
			_handler.erase(it);
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
		std::lock_guard<std::mutex> lock(_mutex);
		handler = _handler;
	}

	for(auto it : handler)
		it.forwardEvent(it.eventHandler, event);
}

template <typename EventType>
template <typename EventHandlerType>
inline void EventChannel<EventType>::ForwardEvent(void* handler, EventType& event)
{
	static_cast<EventHandlerType*>(handler)->HandleEvent(event);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/