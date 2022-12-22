module;
#include <mutex>
#include <vector>
export module bembel.base:Signal;

import :Slot;
import :Types;

export namespace bembel::base {

template <typename... TArgs>
class Signal {
    using Lock = std::lock_guard<std::mutex>;

    using Slot = Delegate<TArgs...>;

  public:
    Signal()                    = default;
    Signal(Signal const& other) = delete;
    ~Signal()                   = default;

    // clang-format off
                       void bind(           void ( *function)(TArgs...))       { bind(Slot(function)); }
    template <class C> void bind(C* object, void (C::*method)(TArgs...))       { bind(Slot(object, method)); }
    template <class C> void bind(C* object, void (C::*method)(TArgs...) const) { bind(Slot(object, method)); }
    template <class C> void bind(C* functor)                                   { bind(Slot(functor)); }
    // clang-format on

    void bind(Slot slot) {
        Lock lock(m_mutex);
        m_slots.push_back(std::move(slot));
    }

    // clang-format off
                       void unbind(void (*function)(TArgs...))                   { unbind(Slot(function)); }
    template <class C> void unbind(C* object, void (C::*method)(TArgs...))       { unbind(Slot(object, method)); }
    template <class C> void unbind(C* object, void (C::*method)(TArgs...) const) { unbind(Slot(object, method)); }
    template <class C> void unbind(C* functor)                                   { unbind(Slot(functor)); }
    // clang-format on

    void unbind(In<Slot> slot) {
        Lock lock(m_mutex);
        auto it = std::find(m_slots.begin(), m_slots.end(), slot);
        if(it != m_slots.end()) m_slots.erase(it);
    }

    void emit(TArgs... args) {
        // Create a temporaty copy of m_slots so m_slots can be modified witin
        // the loop without invalidating the iterator (this allows slots to be
        // removed or added by the methodes/functions bound to this signal).
        SlotVector slots;
        {
            Lock lock(m_mutex);
            slots = m_slots;
        }

        for(auto it : slots) it(args...);
    }
    void operator()(TArgs... args) { emit(args...); }

  private:
    using SlotVector = std::vector<Slot>;

    SlotVector m_slots;

    std::mutex m_mutex;
};

} // namespace bembel::base
