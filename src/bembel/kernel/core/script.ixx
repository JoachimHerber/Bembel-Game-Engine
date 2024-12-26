export module bembel.kernel.core:Task;

import std;
import bembel.base;
import bembel.kernel.assets;
import bembel.kernel.display;
import bembel.kernel.input;

import :System;

export namespace bembel::kernel {
using namespace bembel::base;


template <typename TReturn>
class Script final {
  public:
    class Promise final {
      public:
        using Handle = std::coroutine_handle<Promise>;

        Script [[nodiscard]] get_return_object() { return Script(Handle::from_promise(*this)); }

        std::suspend_never initial_suspend() const noexcept { return {}; }
        std::suspend_never final_suspend() const noexcept { return {}; }

        // Called by the coroutine via co_return;
        void return_value(In<TReturn> v) noexcept
            requires !std::is_same_v<TReturn, void>
        {
            m_return_value = v;
            if(m_parent && !m_parent.done()) m_parent.resume();
        }
        void return_void() noexcept
            requires std::is_same_v<TReturn, void>
        {
            m_return_value = true;
            if(m_parent && !m_parent.done()) m_parent.resume();
        }

        friend struct Awaiter;
        struct Awaiter {
            Awaiter(Promise promise) : m_promise{promise} {}

            // For operator co_await
            void await_suspend(std::coroutine_handle<> handle) noexcept {
                m_promise.m_parent = handle;
            }
            auto await_resume() {
                if constexpr(std::is_same_v<TReturn, void>) {
                    return;
                } else {
                    return m_promise.m_return_value.value();
                }
            }
            bool await_ready() noexcept {
                if constexpr(std::is_same_v<TReturn, void>) {
                    return m_promise.m_return_value;
                } else {
                    return m_promise.m_return_value.has_value();
                }
            }

            void await_cancel() { m_promise.m_parent = std::coroutine_handle<>{}; }

            ScriptPromise* m_promise;
        };

        template <typename TRet>
        auto await_transform(Script<TRet>&& otherScript) {
            return Script<TRet>::Promise::Awaiter();
        }

        void unhandled_exception() { logError("Unhandled exception in coroutine\n"); }

        void abort() {
            if(m_abort) m_abort();
        }

      private:
        using ReturnValueContainer =
            std::conditional<std::is_same_v<TReturn, void>, bool, std::optional<TReturn>>;
        std::function<void>     m_abort;
        ReturnValueContainer    m_return_value;
        std::coroutine_handle<> m_parent{};
    };

    using promise_type = Promise;

  public:
    Script(std::coroutine_handle<promise_type> hndl) : m_hndl(hndl) {}
    ~Script() {
        if(!m_hndl.done()) m_hndl.promise().abort();
        m_hndl.destroy();
    }

    bool isDone() const { return m_hndl.done(); }

    std::optional<TReturn> getResult() const {
        if(m_hndl.done()) { return m_hndl.promise().await_resume(); }
        return {};
    }

  private:
    std::coroutine_handle<promise_type> m_hndl;
};

} // namespace bembel::kernel
