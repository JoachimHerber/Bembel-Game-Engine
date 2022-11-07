module;
#include "bembel/pch.h"
export module bembel.base:Events;

import :Types;

namespace bembel::base {

export class EventChannelBase {
  public:
    EventChannelBase()          = default;
    virtual ~EventChannelBase() = default;
};

export template <typename EventType>
class EventChannel : public EventChannelBase {
    using Lock = std::scoped_lock<std::mutex>;

  public:
    EventChannel()  = default;
    ~EventChannel() = default;

    template <typename EventHandlerType>
    bool addHandler(EventHandlerType* handler) {
        Lock lock(m_mutex);

        for(auto& it : m_event_handler) {
            if(it.event_handler == handler) return false; // handler already registered
        }

        HandlerAdapter adapter;
        adapter.event_handler          = handler;
        adapter.forward_event_function = forwardEvent<EventHandlerType>;
        m_event_handler.push_back(adapter);
        return true;
    }

    template <typename EventHandlerType>
    bool removeHandler(EventHandlerType* handler) {
        Lock lock(m_mutex);

        for(auto it = m_event_handler.begin(); it != m_event_handler.end(); ++it) {
            if(it->event_handler == handler) {
                m_event_handler.erase(it);
                return true;
            }
        }

        return false; // handler not found
    }

    void broadcast(In<EventType> event) {
        std::vector<HandlerAdapter> handler;
        {
            Lock lock(m_mutex);
            handler = m_event_handler;
        }

        for(auto it : handler) it.forward_event_function(it.event_handler, event);
    }

  private:
    template <typename EventHandlerType>
    static void forwardEvent(In<void*> handler, In<EventType> event) {
        static_cast<EventHandlerType*>(handler)->handleEvent(event);
    }
    using ForwardEventFunction = void (*)(In<void*>, In<EventType>);

    struct HandlerAdapter {
        void*                event_handler;
        ForwardEventFunction forward_event_function;
    };

    std::vector<HandlerAdapter> m_event_handler;

    std::mutex m_mutex;
};

export class EventManager final {
    using EventChannelPointer = std::unique_ptr<EventChannelBase>;
    using EventChannelMap     = std::unordered_map<std::type_index, EventChannelPointer>;

  public:
    EventManager()                               = default;
    EventManager(EventManager&&)                 = delete;
    EventManager(EventManager const&)            = delete;
    EventManager& operator=(EventManager&&)      = delete;
    EventManager& operator=(EventManager const&) = delete;
    ~EventManager()                              = default;

    template <typename EventType, typename EventHandlerType>
    bool addHandler(EventHandlerType* handler) {
        auto channel = getChannel<EventType>();
        if(!channel) return false;
        return channel->addHandler(handler);
    }
    template <typename EventType, typename EventHandlerType>
    bool removeHandler(EventHandlerType* handler) {
        auto channel = getChannel<EventType>();
        if(!channel) return false;
        return channel->removeHandler(handler);
    }

    template <typename EventType>
    void broadcast(EventType const& event) {
        auto channel = getChannel<EventType>();
        if(channel) channel->broadcast(event);
    }

    template <typename EventType>
    EventChannel<EventType>* getChannel() {
        auto it = m_channels.find(typeid(EventType));
        if(it != m_channels.end()) {
            return static_cast<EventChannel<EventType>*>(it->second.get());
        }

        auto channel    = std::make_unique<EventChannel<EventType>>();
        auto channelPtr = channel.get();

        m_channels.emplace(typeid(EventType), std::move(channel));

        return channelPtr;
    }

  private:
    EventChannelMap m_channels;
};

} // namespace bembel::base
