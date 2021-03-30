#ifndef BEMBEL_INPUT_SLOT_HPP
#define BEMBEL_INPUT_SLOT_HPP

#include <bembel/library.hpp>

namespace bembel::base {

template <typename... TArgs>
class Slot {
  public:
    Slot(Slot&&);
    Slot(const Slot&);
    Slot(void (*)(TArgs...));
    template <typename C>
    Slot(C* object);
    template <typename C>
    Slot(C* object, void (C::*method)(TArgs...));
    template <typename C>
    Slot(C* object, void (C::*method)(TArgs...) const);

    ~Slot();

    void operator()(TArgs... args);

    bool operator==(const Slot<TArgs...>& oOther);
    bool operator!=(const Slot<TArgs...>& oOther);

    Slot& operator=(const Slot&);

  private:
    using InvokeFunctionType  = void (*)(void*, TArgs...);
    using DeleteFunctionType  = void (*)(void*);
    using CompareFunctionType = bool (*)(void*, void*);
    using CloneFunctionType   = void* (*)(void*);

    void* _target;
    InvokeFunctionType _invoke;
    DeleteFunctionType _delete;
    CompareFunctionType _compare;
    CloneFunctionType _clone;

  private:
    template <typename C>
    struct ObjectMethodPair {
        using MethodPtrType = void (C::*)(TArgs...);

        ObjectMethodPair(C* object, MethodPtrType method)
        : object(object)
        , method(method) {}

        C* object;
        MethodPtrType method;

        static void dispose(void* object);
        static bool compare(void* p1, void* p2);
        static void* clone(void* p);
    };

    template <typename C>
    struct ObjectConstMethodPair {
        using MethodPtrType = void (C::*)(TArgs...) const;

        ObjectConstMethodPair(C* object, MethodPtrType method)
        : object(object)
        , method(method) {}

        C* object;
        MethodPtrType method;

        static void dispose(void* object);
        static bool compare(void* p1, void* p2);
        static void* clone(void* p);
    };

    static void invokeFunction(void* funktion, TArgs... args);

    template <typename FunctorType>
    static void invokeFunctor(void* functor, TArgs... args);

    template <typename C>
    static void invokeMethod(void* p, TArgs... args);
    template <typename C>
    static void invokeConstMethod(void* p, TArgs... args);

    static bool comparePointerAddress(void* object1, void* object2);
    static void* clonePointer(void* p);
};

} // namespace bembel::base
#include "Slot.inl"
#endif // include guards
