export module bembel.base:Awaitable;

import std;
import :Types;
import :Logger;
import :Coroutine;

namespace bembel::base::coro {

export template <typename T>
class Awaitable {
  public:
    Awaitable()                 = default;
    Awaitable(Awaitable const&) = delete;
    Awaitable(Awaitable&&)      = delete;
    
    constexpr bool await_ready() const noexcept { return false; }
    template <typename T>
        requires std::is_base_of_v<PromiseBase, T>
    void           await_suspend(std::coroutine_handle<T> handle) {
        std::scoped_lock lock{m_mutex};
        m_awaiting_coroutines.emplace_back(handle);
    }
    T await_resume() { return m_value; }

    void notify(In<T> value) {
        {
            std::scoped_lock lock{m_mutex};
            if(!m_coroutines_to_notify.empty()) {
                logError("Notify is called while notify is in progress.");
                return;
            }
            m_value = value;
            std::swap(m_awaiting_coroutines, m_coroutines_to_notify);
        }
        for(auto& coroutine : m_coroutines_to_notify) {
            if(coroutine && !coroutine.isDone()) coroutine.resume();
        }
        m_coroutines_to_notify.clear();
    }

  private:
    using CoroHndl = CoroutineHandle<PromiseBase>;

    std::mutex            m_mutex;
    std::vector<CoroHndl> m_awaiting_coroutines;
    std::vector<CoroHndl> m_coroutines_to_notify;
    T                     m_value{};
};

export template <>
class Awaitable<void> {
  public:
    Awaitable()                 = default;
    Awaitable(Awaitable const&) = delete;
    Awaitable(Awaitable&&)      = delete;


    constexpr bool await_ready() const noexcept { return false; }
    template <typename T>
        requires std::is_base_of_v<PromiseBase, T>
    void           await_suspend(std::coroutine_handle<T> handle) {
        std::scoped_lock lock{m_mutex};
        m_awaiting_coroutines.emplace_back(handle);
    }
    void await_resume() {}

    void notify() {
        {
            std::scoped_lock lock{m_mutex};
            if(!m_coroutines_to_notify.empty()) {
                logError("Notify is called while notify is in progress.");
                return;
            }
            std::swap(m_awaiting_coroutines, m_coroutines_to_notify);
        }
        for(auto coroutine : m_coroutines_to_notify) {
            if(coroutine && !coroutine.isDone()) coroutine.resume();
        }
        m_coroutines_to_notify.clear();
    }

  private:
    using CoroHndl = CoroutineHandle<PromiseBase>;

    std::mutex            m_mutex;
    std::vector<CoroHndl> m_awaiting_coroutines;
    std::vector<CoroHndl> m_coroutines_to_notify;
};

} // end of namespace bembel::base
