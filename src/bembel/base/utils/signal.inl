namespace bembel::base {

template <typename... TArgs>
inline Signal<TArgs...>::Signal() {
}

template <typename... TArgs>
inline Signal<TArgs...>::~Signal() {
}

template <typename... TArgs>
inline void Signal<TArgs...>::addSlot(void (*function)(TArgs...)) {
    this->addSlot(Slot<TArgs...>(function));
}

template <typename... TArgs>
template <class C>
inline void Signal<TArgs...>::addSlot(C* objekt, void (C::*method)(TArgs...)) {
    this->addSlot(Slot<TArgs...>(objekt, method));
}

template <typename... TArgs>
template <class C>
inline void Signal<TArgs...>::addSlot(C* objekt, void (C::*method)(TArgs...) const) {
    this->addSlot(Slot<TArgs...>(objekt, method));
}

template <typename... TArgs>
template <class C>
inline void Signal<TArgs...>::addSlot(C* functor) {
    this->addSlot(Slot<TArgs...>(functor));
}

template <typename... TArgs>
inline void Signal<TArgs...>::removeSlot(void (*function)(TArgs...)) {
    this->removeSlot(Slot<TArgs...>(function));
}

template <typename... TArgs>
template <class C>
inline void Signal<TArgs...>::removeSlot(C* objekt, void (C::*method)(TArgs...)) {
    this->removeSlot(Slot<TArgs...>(objekt, method));
}

template <typename... TArgs>
template <class C>
inline void Signal<TArgs...>::removeSlot(C* objekt, void (C::*method)(TArgs...) const) {
    this->removeSlot(Slot<TArgs...>(objekt, method));
}

template <typename... TArgs>
template <class C>
inline void Signal<TArgs...>::removeSlot(C* functor) {
    this->removeSlot(Slot<TArgs...>(functor));
}

template <typename... TArgs>
inline void Signal<TArgs...>::addSlot(Slot<TArgs...>&& slot) {
    std::lock_guard<std::mutex> lock(this->mutex);
    this->slots.push_back(slot);
}

template <typename... TArgs>
inline void Signal<TArgs...>::removeSlot(Slot<TArgs...>&& slot) {
    std::lock_guard<std::mutex> lock(this->mutex);

    auto it = std::find(this->slots.begin(), this->slots.end(), slot);

    if(it != this->slots.end()) this->slots.erase(it);
}

template <typename... TArgs>
void Signal<TArgs...>::emit(TArgs... oArgs) {
    SlotVector slots;
    {
        std::lock_guard<std::mutex> lock(this->mutex);
        slots = this->slots;
    }

    for(auto it : slots) it(oArgs...);
}

} // namespace bembel::base
