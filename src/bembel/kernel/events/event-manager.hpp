#ifndef BEMBEL_KERNEL_EVENTS_EVENTMANAGER_HPP
#define BEMBEL_KERNEL_EVENTS_EVENTMANAGER_HPP

#include "./event-channel.hpp"

namespace bembel::kernel {

class BEMBEL_API EventManager final {
  public:
    EventManager()                    = default;
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;
    ~EventManager()                              = default;

    template <typename EventType, typename EventHandlerType>
    bool addHandler(EventHandlerType* handler);
    template <typename EventType, typename EventHandlerType>
    bool removeHandler(EventHandlerType* handler);

    template <typename EventType>
    void broadcast(const EventType& event);

    template <typename EventType>
    EventChannel<EventType>* getChannel();

  private:
    using EventChannelPointer = std::unique_ptr<EventChannelBase>;
    std::unordered_map<std::type_index, EventChannelPointer> channels;
};

} // namespace bembel::kernel
#include "event-manager.inl"
#endif // include guards
