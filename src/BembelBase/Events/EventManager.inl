/*============================================================================*/
/* INLINE IMPLEMENTETION                                                      */
/*============================================================================*/
namespace bembel {
	
template <typename EventType>
inline void EventManager::Broadcast(EventType& event)
{
	auto channel = GetChannel<EventType>();
	if(channel)
		channel->Broadcast(event);
}

template <typename EventType, typename EventHandlerType>
inline bool EventManager::RemoveHandler(EventHandlerType* handler)
{
	auto channel = GetChannel<EventType>();
	if(!channel)
		return false;
	return channel->RemoveHandler(handler);
}

template <typename EventType, typename EventHandlerType>
inline bool EventManager::AddHandler(EventHandlerType* handler)
{
	auto channel = GetChannel<EventType>();
	if(!channel)
		return false;
	return channel->AddHandler(handler);
}

template <typename EventType>
inline EventChannel<EventType>* EventManager::GetChannel()
{
	unsigned typeID = EventType::GetEventTypeID();

	if(typeID >= _channels.size())
		_channels.resize(typeID+1);

	if(!_channels[typeID])
		_channels[typeID] = new EventChannel<EventType>();

	return static_cast<EventChannel<EventType>*>(_channels[typeID]);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/