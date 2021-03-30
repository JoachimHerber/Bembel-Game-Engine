namespace bembel::kernel {

template <typename EventType>
inline void EventManager::broadcast(const EventType& event) {
    auto channel = this->getChannel<EventType>();
    if(channel) channel->broadcast(event);
}

template <typename EventType, typename EventHandlerType>
inline bool EventManager::removeHandler(EventHandlerType* handler) {
    auto channel = this->getChannel<EventType>();
    if(!channel) return false;
    return channel->removeHandler(handler);
}

template <typename EventType, typename EventHandlerType>
inline bool EventManager::addHandler(EventHandlerType* handler) {
    auto channel = this->getChannel<EventType>();
    if(!channel) return false;
    return channel->addHandler(handler);
}

template <typename EventType>
inline EventChannel<EventType>* EventManager::getChannel() {
    auto& channel = this->channels[typeid(EventType)];

    if(!channel) channel = std::make_unique<EventChannel<EventType>>();

    return static_cast<EventChannel<EventType>*>(channel.get());
}

} // namespace bembel::kernel
