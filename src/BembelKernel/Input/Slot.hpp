#ifndef BEMBEL_SLOT_H
#define BEMBEL_SLOT_H
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

template<typename ... TArgs>
class Slot
{
public:
	Slot(Slot&&);
	Slot(const Slot&);
	Slot(void(*)(TArgs...));
	template <typename C>
	Slot(C* object);
	template <typename C>
	Slot(C* object, void (C::* method)(TArgs...));
	template <typename C>
	Slot(C* object, void (C::* method)(TArgs...) const);

	~Slot();

	void operator()(TArgs... args);

	bool operator==(const Slot<TArgs...>& oOther);
	bool operator!=(const Slot<TArgs...>& oOther);

	Slot& operator= (const Slot&);

private:
	using InvokeFunctionType  = void(*)(void*, TArgs...);
	using DeleteFunctionType  = void(*)(void*);
	using CompareFunctionType = bool(*)(void*, void*);
	using CloneFunctionType   = void*(*)(void*);

	void*               _target;
	InvokeFunctionType  _invoke;
	DeleteFunctionType  _delete;
	CompareFunctionType _compare;
	CloneFunctionType   _clone;

private:
	template <typename C>
	struct ObjectMethodPair
	{
		using MethodPtrType = void(C::*)(TArgs...);

		ObjectMethodPair(C* object, MethodPtrType method)
			: object(object)
			, method(method)
		{}

		C*            object;
		MethodPtrType method;

		static void Delete(void* object);
		static bool Compare(void* p1, void* p2);
		static void* Clone(void* p);
	};

	template <typename C>
	struct ObjectConstMethodPair
	{
		using MethodPtrType = void(C::*)(TArgs...) const;

		ObjectConstMethodPair(C* object, MethodPtrType method)
			: object(object)
			, method(method)
		{}

		C*            object;
		MethodPtrType method;

		static void Delete(void* object);
		static bool Compare(void* p1, void* p2);
		static void* Clone(void* p);
	};

	static void InvokeFunction(void* funktion, TArgs... args);

	template <typename FunctorType>
	static void InvokeFunctor(void* functor, TArgs... args);

	template <typename C>
	static void InvokeMethod(void* p, TArgs... args);
	template <typename C>
	static void InvokeConstMethod(void* p, TArgs... args);
	
	static bool ComparePointerAddress(void* object1, void* object2);
	static void* ClonePointer(void* p);
};

} //end of namespace bembel
/*============================================================================*/
/* INLINE IMPLEMENTATIONS													  */
/*============================================================================*/

#include "Delegate.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
