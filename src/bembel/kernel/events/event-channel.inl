namespace bembel::kernel {

template <typename EventType>
template <typename EventHandlerType>
inline bool EventChannel<EventType>::addHandler(EventHandlerType* handler) {
    std::lock_guard<std::mutex> lock(this->mutex);

    for(auto it : this->event_handler) {
        if(it.event_handler == handler) return false; // handler already registered
    }

    HandlerAdapter adapter;
    adapter.event_handler          = handler;
    adapter.forward_event_function = forwardEvent<EventHandlerType>;
    this->event_handler.push_back(adapter);
    return true;
}

template <typename EventType>
template <typename EventHandlerType>
inline bool EventChannel<EventType>::removeHandler(EventHandlerType* handler) {
    std::lock_guard<std::mutex> lock(this->mutex);

    for(auto it = this->event_handler.begin(); it != this->event_handler.end(); ++it) {
        if(it->event_handler == handler) {
            this->event_handler.erase(it);
            return true;
        }
    }

    return false; // handler not found
}

template <typename EventType>
inline void EventChannel<EventType>::broadcast(const EventType& event) {
    std::vector<HandlerAdapter> handler;
    {
        std::lock_guard<std::mutex> lock(this->mutex);
        handler = this->event_handler;
    }

    for(auto it : handler) it.forward_event_function(it.event_handler, event);
}

template <typename EventType>
template <typename EventHandlerType>
inline void EventChannel<EventType>::forwardEvent(void* handler, const EventType& event) {
    static_cast<EventHandlerType*>(handler)->handleEvent(event);
}

} // namespace bembel::kernel
