#ifndef BEMBEL_KERNEL_EVENTS_CHANNEL_HPP
#define BEMBEL_KERNEL_EVENTS_CHANNEL_HPP

#include <bembel/library.hpp>

namespace bembel::kernel {

class EventChannelBase {
  public:
    EventChannelBase(){};
    virtual ~EventChannelBase(){};
};

template <typename EventType>
class EventChannel : public EventChannelBase {
  public:
    EventChannel(){};
    ~EventChannel(){};

    template <typename EventHandlerType>
    bool addHandler(EventHandlerType* handler);
    template <typename EventHandlerType>
    bool removeHandler(EventHandlerType* handler);

    void broadcast(const EventType& event);

  private:
    template <typename EventHandlerType>
    static void forwardEvent(void*, const EventType&);
    using ForwardEventFunction = void (*)(void*, const EventType&);

    struct HandlerAdapter {
        void* event_handler;
        ForwardEventFunction forward_event_function;
    };

    std::vector<HandlerAdapter> event_handler;

    std::mutex mutex;
};

} // namespace bembel::kernel
#include "event-channel.inl"
#endif // include guards
