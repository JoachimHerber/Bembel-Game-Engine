export module bembel.kernel : EventManager;

import<memory>;
import<map>;
import<typeindex>;

import : EventChannel;

namespace bembel::kernel {

export class EventManager final {
  public:
    EventManager()                    = default;
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;
    ~EventManager()                              = default;

    template <typename EventType, typename EventHandlerType>
    bool addHandler(EventHandlerType* handler) {
        auto channel = this->getChannel<EventType>();
        if(!channel) return false;
        return channel->addHandler(handler);
    }
    template <typename EventType, typename EventHandlerType>
    bool removeHandler(EventHandlerType* handler) {
        auto channel = this->getChannel<EventType>();
        if(!channel) return false;
        return channel->removeHandler(handler);
    }

    template <typename EventType>
    void broadcast(const EventType& event) {
        auto channel = this->getChannel<EventType>();
        if(channel) channel->broadcast(event);
    }

    template <typename EventType>
    EventChannel<EventType>* getChannel() {
        auto& channel = this->channels[typeid(EventType)];

        if(!channel) channel = std::make_unique<EventChannel<EventType>>();

        return static_cast<EventChannel<EventType>*>(channel.get());
    }

  private:
    using EventChannelPointer = std::unique_ptr<EventChannelBase>;
    std::map<std::type_index, EventChannelPointer> channels;
};

} // namespace bembel::kernel
