#ifndef BEMBEL_DELEGATE_H
#define BEMBEL_DELEGATE_H
/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel {

template<typename ... TArgs>
class Delegate
{
public:
	Delegate(Delegate&&);
	Delegate(const Delegate&);
	Delegate(void(*)(TArgs...));
	template <typename C>
	Delegate(C* object);
	template <typename C>
	Delegate(C* object, void (C::* method)(TArgs...));
	template <typename C>
	Delegate(C* object, void (C::* method)(TArgs...) const);

	~Delegate();

	void operator()(TArgs... args);

	bool operator==(const Delegate<TArgs...>& oOther);
	bool operator!=(const Delegate<TArgs...>& oOther);

	Delegate& operator= (const Delegate&);

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
