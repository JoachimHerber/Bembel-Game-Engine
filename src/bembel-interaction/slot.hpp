/******************************************************************************/
/* ************************************************************************** */
/* *                                                                        * */
/* *    MIT License                                                         * */
/* *                                                                        * */
/* *   Copyright(c) 2017 Joachim Herber                                     * */
/* *                                                                        * */
/* *   Permission is hereby granted, free of charge, to any person          * */
/* *   obtaining copy of this software and associated documentation files   * */
/* *   (the "Software"), to deal in the Software without restriction,       * */
/* *   including without limitation the rights to use, copy, modify, merge, * */
/* *   publish, distribute, sublicense, and/or sell copies of the Software, * */
/* *   and to permit persons to whom the Software is furnished to do so,    * */
/* *   subject to the following conditions :                                * */
/* *                                                                        * */
/* *   The above copyright notice and this permission notice shall be       * */
/* *   included in all copies or substantial portions of the Software.      * */
/* *                                                                        * */
/* *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      * */
/* *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   * */
/* *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                * */
/* *   NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   * */
/* *   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   * */
/* *   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    * */
/* *   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     * */
/* *   SOFTWARE.                                                            * */
/* *                                                                        * */
/* ************************************************************************** */
/******************************************************************************/

#ifndef BEMBEL_INTERACTION_SLOT_H_
#define BEMBEL_INTERACTION_SLOT_H_
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

#include "Slot.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
