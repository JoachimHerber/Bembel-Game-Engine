export module bembel.base:Coroutine;

import std;
import :Types;

export namespace bembel::base::coro {
/*
  https://en.cppreference.com/w/cpp/language/coroutines

  Coroutine Execution:
   *    allocate and initialize coroutine state object
   *    call PromiseObject constructor.
   *    return_object := promise.get_return_object()
   *    co_await promise.initial_suspend();
   *    starts executing the body of the coroutine
   *    coroutine reaches a suspension point -> return return_object

  co_await (expr): suspends a coroutine and returns control to the caller.
    *   awaitable := promise.await_transform(expr) || expr
      !   await_transform(expr) is not applied to an initial suspend point, a final suspend point,
  or a yield expression
    *   awaiter   := awaitable.operator co_await() || operator co_await( awaitable ) || awaitable
    *   if not awaiter.await_ready()
      -   suspend := awaiter.await_suspend(handle_of_awaiting_coroutine)
      -   if suspend == false ->  resumes the current coroutine
      -   if suspend is coroutine handle -> ret.resume()
    *   return awaiter.await_resume()

  co_yield: eqivalent to co_await promise.yield_value(expr)

  co_return (expr):
    *   if expr is void -> promise.return_void()
    *   else            -> promise.return_value(expr)
    *   co_awaits promise.final_suspend()

 */

// clang-format off
export template <typename T>
concept AwaiterType = requires(T awaiter, std::coroutine_handle<> coro) {
    { awaiter.await_ready()       } -> std::convertible_to<bool>;
    { awaiter.await_suspend(coro) };
    { awaiter.await_resume()      };
};

export template <typename T>
concept AwaitableType = AwaiterType<T>
  || requires(T a) { { a.operator co_await() } -> AwaiterType;}
  || requires(T a) { { operator co_await(a)  } -> AwaiterType;};

export template <typename T>
concept PromiseType = requires(T promise) {
    { promise.initial_suspend()   } -> AwaitableType;
    { promise.final_suspend()     } -> AwaitableType;
    { promise.get_return_object() };
};
// clang-format on

export template <typename TPromise>
//    requires requires(TPromise p) {{ p.reference_count };}
class CoroutineHandle {
  public:
    CoroutineHandle() = default;
    ~CoroutineHandle() {
        if(m_hndl) {
            auto ref_count = --m_hndl.promise().reference_count;
            if(ref_count == 0) m_hndl.destroy();
        }
    }

    friend static void swap(CoroutineHandle& lhs, CoroutineHandle& rhs) {
        std::swap(lhs.m_hndl, rhs.m_hndl);
    }

    CoroutineHandle(std::coroutine_handle<TPromise> hndl) : m_hndl{hndl} {
        if(m_hndl) ++m_hndl.promise().reference_count;
    }
    CoroutineHandle(CoroutineHandle&& other) { swap(*this, other); }
    CoroutineHandle(CoroutineHandle const& other) : CoroutineHandle{other.m_hndl} {}

    CoroutineHandle& operator=(std::coroutine_handle<TPromise> hndl) {
        CoroutineHandle tmp{hndl};
        swap(*this, tmp);
        return *this;
    }
    CoroutineHandle& operator=(CoroutineHandle&& other) {
        swap(*this, other);
        return *this;
    }
    CoroutineHandle& operator=(CoroutineHandle const& other) {
        *this = other.m_hndl;
        return *this;
    }

    std::coroutine_handle<TPromise> release() {
        std::coroutine_handle<TPromise> hndl = m_hndl;
        m_hndl                               = nullptr;
        if(hndl) hndl.promise().decrementReferenceCount();
        return hndl;
    }

  private:
    std::coroutine_handle<TPromise> m_hndl{nullptr};
};

export class PromiseBase {
  public:
    friend class DefaultSuspend;

    struct FinalSuspend {
        bool await_ready() const noexcept { return false; }
        template <typename TPromise>
            requires std::is_base_of_v<PromiseBase, TPromise>
        auto await_suspend(std::coroutine_handle<TPromise> coro) noexcept {
            auto continuation = coro.promise().continuation.release();
            auto ref_count    = --coro.promise().reference_count;
            if(ref_count == 0) coro.destroy();
            return coro.promise().continuation.release();
        }
        void await_resume() noexcept {}
    };

    std::suspend_always initial_suspend() const noexcept { return {}; }
    FinalSuspend        final_suspend() const noexcept { return {}; }

  public:
    std::atomic<u64>             reference_count{1};
    CoroutineHandle<PromiseBase> continuation{};
};

export template <typename T>
class Task;

export template <typename T>
class TaskPromise : public PromiseBase {
  public:
    using Handle = std::coroutine_handle<TaskPromise>;

  public:
    TaskPromise()                              = default;
    ~TaskPromise()                             = default;
    TaskPromise(TaskPromise&&)                 = delete;
    TaskPromise(TaskPromise const&)            = delete;
    TaskPromise& operator=(TaskPromise&&)      = delete;
    TaskPromise& operator=(TaskPromise const&) = delete;

    Task<T> get_return_object();

    void unhandled_exception() noexcept {
        m_value.emplace<std::exception_ptr>(std::current_exception());
    }
    template <typename TValue>
        requires std::is_convertible_v<TValue&&, T>
    void return_value(TValue&& value) noexcept(std::is_nothrow_constructible_v<T, TValue&&>) {
        m_value.emplace<T>(std::forward<TValue>(value));
    }

  private:
    std::variant<std::monostate, T, std::exception_ptr> m_value;
};

export template <typename T>
class Task {};

export template <typename T>
Task<T> TaskPromise<T>::get_return_object() {
    return {Handle::from_promise(*this)};
}

} // namespace bembel::base::coro
