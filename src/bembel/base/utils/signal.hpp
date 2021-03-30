#ifndef BEMBEL_INPUT_SIGNAL_HPP
#define BEMBEL_INPUT_SIGNAL_HPP

#include "./slot.hpp"

namespace bembel::base {

template <typename... TArgs>
class Signal {
  public:
    Signal();
    Signal(const Signal& other) = delete;
    ~Signal();

    void addSlot(void (*function)(TArgs...));
    template <class C>
    void addSlot(C* object, void (C::*method)(TArgs...));
    template <class C>
    void addSlot(C* object, void (C::*method)(TArgs...) const);
    template <class C>
    void addSlot(C* functor);

    void addSlot(Slot<TArgs...>&& slot);

    void removeSlot(void (*function)(TArgs...));
    template <class C>
    void removeSlot(C* object, void (C::*method)(TArgs...));
    template <class C>
    void removeSlot(C* object, void (C::*method)(TArgs...) const);
    template <class C>
    void removeSlot(C* functor);

    void removeSlot(Slot<TArgs...>&& slot);

    void emit(TArgs... args);

  private:
    using SlotVector = std::vector<Slot<TArgs...>>;

    SlotVector slots;

    std::mutex mutex;
};

} // namespace bembel::base
#include "Signal.inl"
#endif // include guards
