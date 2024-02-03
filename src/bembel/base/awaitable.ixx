module;
#include <coroutine>
#include <mutex>
#include <vector>
export module bembel.base:Awaitable;

import :Types;
import :Logger;

namespace bembel::base {

export template <typename T>
class Awaitable {
  public:
    Awaitable()                 = default;
    Awaitable(Awaitable const&) = delete;
    Awaitable(Awaitable&&)      = delete;

    constexpr bool await_ready() const noexcept { return false; }
    void           await_suspend(std::coroutine_handle<> handle) {
        std::scoped_lock lock{m_mutex};
        m_awaiting_coroutines.push_back(handle);
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
        for(auto coroutine : m_coroutines_to_notify) {
            if(coroutine && !coroutine.done()) coroutine.resume();
        }
        m_coroutines_to_notify.clear();
    }

  private:
    std::mutex                           m_mutex;
    std::vector<std::coroutine_handle<>> m_awaiting_coroutines;
    std::vector<std::coroutine_handle<>> m_coroutines_to_notify;
    T                                    m_value{};
};

export template <typename T>
class Awaitable<T&> {
  public:
    Awaitable()                 = default;
    Awaitable(Awaitable const&) = delete;
    Awaitable(Awaitable&&)      = delete;

    constexpr bool await_ready() const noexcept { return false; }
    void           await_suspend(std::coroutine_handle<> handle) {
        std::scoped_lock lock{m_mutex};
        m_awaiting_coroutines.push_back(handle);
    }
    T& await_resume() { return *m_value; }

    void notify(T& value) {
        {
            std::scoped_lock lock{m_mutex};
            if(!m_coroutines_to_notify.empty()) {
                logError("Notify is called while notify is in progress.");
                return;
            }
            m_value = &value;
            std::swap(m_awaiting_coroutines, m_coroutines_to_notify);
        }
        for(auto coroutine : m_coroutines_to_notify) {
            if(coroutine && !coroutine.done()) coroutine.resume();
        }
        m_coroutines_to_notify.clear();
    }

  private:
    std::mutex                           m_mutex;
    std::vector<std::coroutine_handle<>> m_awaiting_coroutines;
    std::vector<std::coroutine_handle<>> m_coroutines_to_notify;
    T*                                   m_value{};
};

export template <>
class Awaitable<void> {
  public:
    Awaitable()                 = default;
    Awaitable(Awaitable const&) = delete;
    Awaitable(Awaitable&&)      = delete;

    constexpr bool await_ready() const noexcept { return false; }
    void           await_suspend(std::coroutine_handle<> handle) {
        std::scoped_lock lock{m_mutex};
        m_awaiting_coroutines.push_back(handle);
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
            if(coroutine && !coroutine.done()) coroutine.resume();
        }
        m_coroutines_to_notify.clear();
    }

  private:
    std::mutex                           m_mutex;
    std::vector<std::coroutine_handle<>> m_awaiting_coroutines;
    std::vector<std::coroutine_handle<>> m_coroutines_to_notify;
};

} // end of namespace bembel::base
