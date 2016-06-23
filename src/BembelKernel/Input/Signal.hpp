#ifndef BEMBEL_SIGNAL_H
#define BEMBEL_SIGNAL_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include "Delegate.hpp"

#include <mutex>
#include <vector>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

template<typename ... TArgs>
class Signal
{
public:
	Signal();
	Signal(const Signal& other) = delete;
	~Signal();

	void AddSlot(void(*function)(TArgs...));
	template<class C>
	void AddSlot(C* object, void(C::* method)(TArgs...));
	template<class C>
	void AddSlot(C* object, void(C::* method)(TArgs...) const);
	template<class C>
	void AddSlot(C* functor);

	void AddSlot(Slot<TArgs...>&& slot);

	void RemoveSlot(void(*function)(TArgs...));
	template<class C>
	void RemoveSlot(C* object, void(C::* method)(TArgs...));
	template<class C>
	void RemoveSlot(C* object, void(C::* method)(TArgs...) const);
	template<class C>
	void RemoveSlot(C* functor);

	void RemoveSlot(Slot<TArgs...>&& slot);

	void Emit(TArgs ... args);

private:
	using SlotVector = std::vector<Slot<TArgs...>>;

	SlotVector _slots;

	std::mutex _mutex;
};

} //end of namespace bembel
/*============================================================================*/
/* INLINE IMPLEMENTATIONS													  */
/*============================================================================*/

#include "Signal.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
