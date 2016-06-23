
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
	AddSlot(Delegate<TArgs...>(function));
}

template<typename ... TArgs>
template<class C>
inline void Signal<TArgs ...>::AddSlot(
	C*  objekt, void(C::* method)(TArgs...))
{
	AddSlot(Delegate<TArgs...>(objekt, method));
}

template<typename ... TArgs>
template<class C>
inline void Signal<TArgs ...>::AddSlot(
	C* objekt, void(C::* method)(TArgs...) const)
{
	AddSlot(Delegate<TArgs...>(objekt, method));
}

template<typename ... TArgs>
template<class C>
inline void Signal<TArgs ...>::AddSlot(C* functor)
{
	AddSlot(Delegate<TArgs...>(functor));
}

template<typename ... TArgs>
inline void Signal<TArgs ...>::RemoveSlot(
	void(*function)(TArgs...))
{
	RemoveSlot(Delegate<TArgs...>(function));
}

template<typename ... TArgs>
template<class C>
inline void Signal<TArgs ...>::RemoveSlot(
	C* objekt, void(C::* method)(TArgs...))
{
	RemoveSlot(Delegate<TArgs...>(objekt, method));
}

template<typename ... TArgs>
template<class C>
inline void Signal<TArgs ...>::RemoveSlot(
	C* objekt, void(C::* method)(TArgs...) const)
{
	RemoveSlot(Delegate<TArgs...>(objekt, method));
}

template<typename ... TArgs>
template<class C>
inline void Signal<TArgs ...>::RemoveSlot(C* functor)
{
	RemoveSlot(Delegate<TArgs...>(functor));
}

template<typename ... TArgs>
inline void Signal<TArgs ...>::AddSlot(Delegate<TArgs...>&& slot)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_slots.push_back(slot);
}

template<typename ... TArgs>
inline void Signal<TArgs ...>::RemoveSlot(Delegate<TArgs...>&& slot)
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