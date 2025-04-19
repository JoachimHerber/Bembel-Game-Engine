export module bembel.base:Coroutine;

import std;
import :Types;

export namespace bembel::base::coro {
/*
  https://en.cppreference.com/w/cpp/language/coroutines

  Coroutine Execution:
    allocate and initialize coroutine state object
    call PromiseObject constructor.
    return_object := promise.get_return_object()
    co_await promise.initial_suspend();
      if suspend
        return return_object
      else
        starts executing the body of the coroutine
        coroutine reaches a suspension point -> return return_object

  co_await (expr): suspends a coroutine and returns control to the caller.
    awaitable := promise.await_transform(expr) || expr
    // await_transform(expr) is not applied to an initial suspend point, a final suspend point, or a
  yield expression awaiter   := awaitable.operator co_await() || operator co_await( awaitable ) ||
  awaitable if not awaiter.await_ready() suspend
  awaiter.await_suspend(handle_of_awaiting_coroutine) if suspend == false resumes the current
  coroutine immediately if suspend is a coroutine_handle suspend.resume() return
  awaiter.await_resume()

  co_yield (expr): eqivalent to co_await promise.yield_value(expr)

  co_return (expr):
    if expr is void
      promise.return_void()
    else
      promise.return_value(expr)
    co_await promise.final_suspend()
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
// requires requires(TPromise p) { { p.reference_count } -> std::same_as<std::atomic<u64>>; }
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
    template <typename T>
        requires std::is_base_of_v<TPromise, T>
    CoroutineHandle(std::coroutine_handle<T> hndl) 
        : CoroutineHandle{std::coroutine_handle<TPromise>::from_address(hndl.address())}
    {}
    CoroutineHandle(CoroutineHandle&& other) { swap(*this, other); }
    CoroutineHandle(CoroutineHandle const& other) : CoroutineHandle{other.m_hndl} {}

    CoroutineHandle& operator=(std::coroutine_handle<TPromise> hndl) {
        CoroutineHandle tmp{hndl};
        swap(*this, tmp);
        return *this;
    }
    template <typename T>
        requires std::is_base_of_v<TPromise, T>
    CoroutineHandle& operator=(std::coroutine_handle<T> hndl) {
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
        if(hndl) --hndl.promise().reference_count;
        return hndl;
    }

    std::coroutine_handle<TPromise> get() { return m_hndl; }

    operator bool() const { return m_hndl.operator bool(); }

    void resume() const noexcept { m_hndl.resume(); }
    bool isDone() const noexcept { return m_hndl.done(); }

    TPromise& getPromise() const noexcept { return m_hndl.promise(); }

    static CoroutineHandle fromPromise(TPromise* promis) {
        return {std::coroutine_handle<TPromise>::from_promise(*promis)};
    }

  private:
    std::coroutine_handle<TPromise> m_hndl{nullptr};
};

export class PromiseBase {
  public:
    friend class DefaultSuspend;

    PromiseBase()          = default;
    virtual ~PromiseBase() = default;

    struct FinalSuspend {
        bool await_ready() const noexcept { return false; }
        template <typename TPromise>
            requires std::is_base_of_v<PromiseBase, TPromise>
        std::coroutine_handle<> await_suspend(std::coroutine_handle<TPromise> coro) noexcept {
            std::coroutine_handle<> previous = coro.promise().continuation.release();
            if(coro.promise().reference_count == 0) coro.destroy();
            if(previous)
                return previous;
            else
                return std::noop_coroutine();
        }
        void await_resume() noexcept {}
    };

    FinalSuspend final_suspend() const noexcept { return {}; }

  public:
    std::atomic<u64>             reference_count{0};
    CoroutineHandle<PromiseBase> continuation{};
};

export template <typename T, bool INITIAL_SUSPEND>
class Promise : public PromiseBase {
  public:
    using Handle = CoroutineHandle<Promise>;
    using Result = std::conditional_t<PASS_BY_VALUE<T>::value, T const, T const&>;
    using InitialSuspend =
        std::conditional_t<INITIAL_SUSPEND, std::suspend_always, std::suspend_never>;

  public:
    Promise()  = default;
    ~Promise() = default;

    InitialSuspend initial_suspend() const noexcept { return {}; }

    void unhandled_exception() noexcept {
        m_data.emplace<std::exception_ptr>(std::current_exception());
    }
    void rethrowUnhandledExceptions() {
        if(std::holds_alternative<std::exception_ptr>(m_data))
            std::rethrow_exception(std::get<std::exception_ptr>(m_data));
    }

    template <typename TValue>
        requires std::is_convertible_v<TValue&&, T>
    void return_value(TValue&& value) noexcept(std::is_nothrow_constructible_v<T, TValue&&>) {
        m_data.emplace<T>(std::forward<TValue>(value));
    }

    template <typename TValue>
        requires std::is_convertible_v<TValue&&, T>
    std::suspend_always yield_value(TValue&& value
    ) noexcept(std::is_nothrow_constructible_v<T, TValue&&>) {
        m_data.emplace<T>(std::forward<TValue>(value));
        return {};
    }

    bool   hasResult() const noexcept { return std::holds_alternative<T>(m_data); }
    Result getResult() const { return std::get<T>(m_data); }

  private:
    std::variant<std::monostate, T, std::exception_ptr> m_data;
};

export template <bool INITIAL_SUSPEND>
class Promise<void, INITIAL_SUSPEND> : public PromiseBase {
  public:
    using Handle = CoroutineHandle<Promise>;
    using Result = void;
    using InitialSuspend =
        std::conditional_t<INITIAL_SUSPEND, std::suspend_always, std::suspend_never>;

  public:
    Promise()  = default;
    ~Promise() = default;

    InitialSuspend initial_suspend() const noexcept { return {}; }

    void unhandled_exception() noexcept { m_exception.emplace(std::current_exception()); }
    void rethrowUnhandledExceptions() {
        if(m_exception.has_value()) std::rethrow_exception(m_exception.value());
    }

    void return_void() noexcept {}

  private:
    std::optional<std::exception_ptr> m_exception;
};

export template <typename TPromise>
struct TaskAwaiter {
    using Handle = TPromise::Handle;
    using Result = TPromise::Result;

    TaskAwaiter(Handle const& hndl) : m_hndl(hndl) {}
    bool await_ready() const { return m_hndl.isDone(); }
    template <typename T>
        requires std::is_base_of_v<PromiseBase, T>
    std::coroutine_handle<> await_suspend(std::coroutine_handle<T> other_coro) {
        m_hndl.getPromise().continuation = other_coro;
        return m_hndl.get();
    }
    Result await_resume() {
        m_hndl.getPromise().rethrowUnhandledExceptions();
        if constexpr(std::is_same_v<Result, void>)
            return;
        else
            return m_hndl.getPromise().getResult();
    }
    Handle m_hndl;
};

export template <
    typename TReturn = void,
    typename TPromise = Promise<TReturn, false>>
class Task {
  public:
    class Promise : public TPromise {
      public:
        Task get_return_object() { return {TPromise::Handle::fromPromise(this)}; }
    };
    using promise_type = Promise;
    using Handle       = TPromise::Handle;
    using Awaiter      = TaskAwaiter<TPromise>;
    using Result       = TPromise::Result;

    Task()                             = default;
    Task(Task const& other)            = default;
    Task(Task&& other)                 = default;
    Task& operator=(Task const& other) = default;
    Task& operator=(Task&& other)      = default;

    Task(Handle&& hndl) : m_hndl{std::move(hndl)} {}

    TPromise& getPromise() const { return m_hndl.getPromise(); }

    void resume() const {
        m_hndl.getPromise().rethrowUnhandledExceptions();
        m_hndl.resume();
        m_hndl.getPromise().rethrowUnhandledExceptions();
    }
    bool isDone() const { return m_hndl.isDone(); }

    bool hasResult() const
        requires !std::is_same_v<TReturn, void>
    {
        return m_hndl.getPromise().hasResult();
    }
    Result getResult() const
        requires !std::is_same_v<TReturn, void>
    {
        return m_hndl.getPromise().getResult();
    }

    Result operator()() const {
        resume();
        return getResult();
    }

    Awaiter operator co_await() const {
        m_hndl.getPromise().rethrowUnhandledExceptions();
        Awaiter awaiter(m_hndl);
        return awaiter;
    }

  protected:
    Handle m_hndl;
};

export template <typename TReturn = void>
using InitialSuspendTask = Task<TReturn, Promise<TReturn, true>>;


} // namespace bembel::base::coro
