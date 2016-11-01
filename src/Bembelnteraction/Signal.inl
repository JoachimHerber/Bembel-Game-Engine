
/*============================================================================*/
/* INLINE IMPLEMENTETION                                                      */
/*============================================================================*/
namespace bembel {

template<typename ... TArgs>
inline Signal<TArgs ...>::Signal()
{}

template<typename ... TArgs>
inline Signal<TArgs ...>::~Signal()
{}

template<typename ... TArgs>
inline void Signal<TArgs ...>::AddSlot(void(*function)(TArgs...))
{
	AddSlot(Slot<TArgs...>(function));
}

template<typename ... TArgs>
template<class C>
inline void Signal<TArgs ...>::AddSlot(
	C*  objekt, void(C::* method)(TArgs...))
{
	AddSlot(Slot<TArgs...>(objekt, method));
}

template<typename ... TArgs>
template<class C>
inline void Signal<TArgs ...>::AddSlot(
	C* objekt, void(C::* method)(TArgs...) const)
{
	AddSlot(Slot<TArgs...>(objekt, method));
}

template<typename ... TArgs>
template<class C>
inline void Signal<TArgs ...>::AddSlot(C* functor)
{
	AddSlot(Slot<TArgs...>(functor));
}

template<typename ... TArgs>
inline void Signal<TArgs ...>::RemoveSlot(
	void(*function)(TArgs...))
{
	RemoveSlot(Slot<TArgs...>(function));
}

template<typename ... TArgs>
template<class C>
inline void Signal<TArgs ...>::RemoveSlot(
	C* objekt, void(C::* method)(TArgs...))
{
	RemoveSlot(Slot<TArgs...>(objekt, method));
}

template<typename ... TArgs>
template<class C>
inline void Signal<TArgs ...>::RemoveSlot(
	C* objekt, void(C::* method)(TArgs...) const)
{
	RemoveSlot(Slot<TArgs...>(objekt, method));
}

template<typename ... TArgs>
template<class C>
inline void Signal<TArgs ...>::RemoveSlot(C* functor)
{
	RemoveSlot(Slot<TArgs...>(functor));
}

template<typename ... TArgs>
inline void Signal<TArgs ...>::AddSlot(Slot<TArgs...>&& slot)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_slots.push_back(slot);
}

template<typename ... TArgs>
inline void Signal<TArgs ...>::RemoveSlot(Slot<TArgs...>&& slot)
{
	std::lock_guard<std::mutex> lock(_mutex);

	auto it = std::find(_slots.begin(), _slots.end(), slot);

	if (it != _slots.end())
		_slots.erase(it);
}

template<typename ... TArgs>
void Signal<TArgs ...>::Emit(TArgs ... oArgs)
{
	SlotVector slots;
	{
		std::lock_guard<std::mutex> lock(_mutex);
		slots = _slots;
	}

	for(auto it : slots)
		it(oArgs...);
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/