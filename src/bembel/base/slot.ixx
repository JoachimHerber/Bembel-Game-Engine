export module bembel.base:Slot;

export namespace bembel::base {

template <typename... TArgs>
class Delegate {
  public:
    Delegate(Delegate&& other)
      : m_target(other.m_target)
      , m_invoke_func(other.m_invoke_func)
      , m_delete_func(other.m_delete_func)
      , m_compare_func(other.m_compare_func)
      , m_clone_func(other.m_clone_func) {
        // the _object is now ours, so we have to make sure,
        // that it won't be delete by the other delegete
        other.m_delete_func = nullptr;
    }

    Delegate(Delegate const& other)
      : m_target(other.m_clone_func(other.m_target))
      , m_invoke_func(other.m_invoke_func)
      , m_delete_func(other.m_delete_func)
      , m_compare_func(other.m_compare_func)
      , m_clone_func(other.m_clone_func) {}

    Delegate(void (*function)(TArgs...))
      : m_target(function)
      , m_invoke_func(&invokeFunction)
      , m_delete_func(nullptr)
      , m_compare_func(&comparePointerAddress)
      , m_clone_func(&clonePointer) {}

    template <typename C>
    Delegate(C* functor)
      : m_target(functor)
      , m_invoke_func(&invokeFunctor<C>)
      , m_delete_func(nullptr)
      , m_compare_func(&comparePointerAddress)
      , m_clone_func(&clonePointer) {}

    template <typename C>
    Delegate(C* object, void (C::*method)(TArgs...))
      : m_target(new ObjectMethodPair<C>(object, method))
      , m_invoke_func(&ObjectMethodPair<C>::invoke)
      , m_delete_func(&ObjectMethodPair<C>::dispose)
      , m_compare_func(&ObjectMethodPair<C>::compare)
      , m_clone_func(&ObjectMethodPair<C>::clone) {}

    template <typename C>
    Delegate(C* object, void (C::*method)(TArgs...) const)
      : m_target(new ObjectConstMethodPair<C>(object, method))
      , m_invoke_func(&ObjectConstMethodPair<C>::invoke)
      , m_delete_func(&ObjectConstMethodPair<C>::dispose)
      , m_compare_func(&ObjectConstMethodPair<C>::compare)
      , m_clone_func(&ObjectConstMethodPair<C>::clone) {}

    ~Delegate() {
        if(m_delete_func) m_delete_func(m_target);
    }

    Delegate& operator=(Delegate const& other) {
        if(m_delete_func) m_delete_func(m_target);

        m_target       = other.m_clone_func(other.m_target);
        m_invoke_func  = other.m_invoke_func;
        m_delete_func  = other.m_delete_func;
        m_compare_func = other.m_compare_func;
        m_clone_func   = other.m_clone_func;

        return *this;
    }

    void operator()(TArgs... args) { m_invoke_func(m_target, args...); }

    bool operator==(Delegate<TArgs...> const& other) {
        if(m_compare_func != other.m_compare_func) return false;
        // Only compare Objects if both Slots use the same CompareFunction
        return m_compare_func(m_target, other.m_target);
    }
    bool operator!=(Delegate<TArgs...> const& other) { return !operator==(other); }

  private:
    using InvokeFunctionType  = void (*)(void*, TArgs...);
    using DeleteFunctionType  = void (*)(void*);
    using CompareFunctionType = bool (*)(void*, void*);
    using CloneFunctionType   = void* (*)(void*);

    void*               m_target;
    InvokeFunctionType  m_invoke_func;
    DeleteFunctionType  m_delete_func;
    CompareFunctionType m_compare_func;
    CloneFunctionType   m_clone_func;

  private:
    static void invokeFunction(void* function, TArgs... args) {
        static_cast<void (*)(TArgs...)>(function)(args...);
    }

    template <typename FunctorType>
    static void invokeFunctor(void* functor, TArgs... args) {
        (*static_cast<FunctorType*>(functor))(args...);
    }

    template <typename C>
    struct ObjectMethodPair {
        using MethodPtrType = void (C::*)(TArgs...);

        ObjectMethodPair(C* object, MethodPtrType method) : object(object), method(method) {}

        C*            object;
        MethodPtrType method;

        static void invoke(void* p, TArgs... args) {
            auto pair = static_cast<ObjectMethodPair<C>*>(p);
            ((pair->object)->*(pair->method))(args...);
        }
        static void dispose(void* object) { delete static_cast<ObjectMethodPair<C>*>(object); }
        static bool compare(void* p1, void* p2) {
            auto pair1 = static_cast<ObjectMethodPair<C>*>(p1);
            auto pair2 = static_cast<ObjectMethodPair<C>*>(p2);
            return pair1->object == pair2->object && pair1->method == pair2->method;
        }
        static void* clone(void* p) {
            auto* pair = static_cast<ObjectMethodPair<C>*>(p);
            return new ObjectMethodPair<C>(pair->object, pair->method);
        }
    };

    template <typename C>
    struct ObjectConstMethodPair {
        using MethodPtrType = void (C::*)(TArgs...) const;

        ObjectConstMethodPair(C* object, MethodPtrType method) : object(object), method(method) {}

        C*            object;
        MethodPtrType method;

        static void invoke(void* p, TArgs... args) {
            auto pair = static_cast<ObjectConstMethodPair<C>*>(p);
            ((pair->object)->*(pair->method))(args...);
        }
        static void dispose(void* object) { delete static_cast<ObjectConstMethodPair<C>*>(object); }
        static bool compare(void* p1, void* p2) {
            auto pair1 = static_cast<ObjectConstMethodPair<C>*>(p1);
            auto pair2 = static_cast<ObjectConstMethodPair<C>*>(p2);
            return pair1->object == pair2->object && pair1->method == pair2->method;
        }
        static void* clone(void* p) {
            auto* pair = static_cast<ObjectConstMethodPair<C>*>(p);
            return new ObjectConstMethodPair<C>(pair->object, pair->method);
        }
    };

    static bool  comparePointerAddress(void* object1, void* object2) { return object1 == object2; }
    static void* clonePointer(void* p) { return p; }
};

} // namespace bembel::base
