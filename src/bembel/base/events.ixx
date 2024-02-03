module;
#include <mutex>
#include <typeindex>
#include <unordered_map>
#include <vector>
export module bembel.base:Events;

import :Types;
import :Awaitable;

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

    bool removeHandler(void* handler) {
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

        m_awaitable.notify(event);
    }

    Awaitable<EventType>& operator co_await() { return m_awaitable; }

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

    Awaitable<EventType> m_awaitable;
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
    template <typename EventType>
    bool removeHandler(void* handler) {
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

export namespace events {
    template <typename EventType>
    EventChannel<EventType>& getEventChannel() {
        static EventChannel<EventType> instance;
        return instance;
    }

    template <typename EventType, typename EventHandlerType>
    void addHandler(EventHandlerType* handler) {
        getEventChannel<EventType>().addHandler(handler);
    }
    template <typename EventType>
    void removeHandler(void* handler) {
        getEventChannel<EventType>().removeHandler(handler);
    }

    template <typename EventType>
    void broadcast(EventType const& event) {
        getEventChannel<EventType>().broadcast(event);
    }
    template <typename EventType, typename... TArgs>
    void broadcast(TArgs&&... args) {
        getEventChannel<EventType>().broadcast(EventType{std::forward<TArgs>(args)...});
    }

    export template <typename EventType>
    class Awaiter {
      public:
        Awaitable<EventType>& operator co_await() {
            return getEventChannel<EventType>().operator co_await();
        }
    };

} // namespace events

export template <class THandler, typename TEvent>
concept isEventHandler = true;
// requires(THandler h, TEvent const e) {
//     { h.handleEvent(e) };
// };

export template <typename... TEvents>
class EventHandlerGuard final {
  public:
    template <class THandler>
        requires(... && isEventHandler<THandler, TEvents>)
    EventHandlerGuard(THandler* handler) : m_handler(handler) {
        (events::addHandler<TEvents>(handler), ...);
    }
    ~EventHandlerGuard() { (events::removeHandler<TEvents>(m_handler), ...); }

  private:
    void* m_handler;
};

} // namespace bembel::base
