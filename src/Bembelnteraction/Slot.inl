/*============================================================================*/
/* INLINE IMPLEMENTETION                                                      */
/*============================================================================*/
namespace bembel {
	
template<typename ... TArgs>
inline Slot<TArgs...>::Slot(Slot&& other)
	: _target(other._target)
	, _invoke(other._invoke)
	, _delete(other._delete)
	, _compare(other._compare)
	, _clone(other._clone)
{
	// the _object is now ours, so we have to make sure,
	// that it won't be delete by the other delegete
	other._delete = nullptr;
}

template<typename ... TArgs>
inline Slot<TArgs...>::Slot(const Slot& other)
	: _target(other._clone(other._target))
	, _invoke(other._invoke)
	, _delete(other._delete)
	, _compare(other._compare)
	, _clone(other._clone)
{}

template<typename ... TArgs>
inline Slot<TArgs...>::Slot(void(*funktion)(TArgs...))
	: _target(funktion)
	, _invoke(&InvokeFunction)
	, _delete(nullptr)
	, _compare(&ComparePointerAddress)
	, _clone(&ClonePointer)
{}

template <typename ... TArgs>
template <typename FunctorType>
inline Slot<TArgs...>::Slot(FunctorType* functor)
	: _target(functor)
	, _invoke(&InvokeFunctor<FunctorType>)
	, _delete(nullptr)
	, _compare(&ComparePointerAddress)
	, _clone(&ClonePointer)
{}

template <typename ... TArgs>
template <typename C>
inline Slot<TArgs...>::Slot(C* object, void (C::* method)(TArgs...))
	: _target(new ObjectMethodPair<C>(object, method))
	, _invoke(&InvokeMethod<C>)
	, _delete(&ObjectMethodPair<C>::Delete)
	, _compare(&ObjectMethodPair<C>::Compare)
	, _clone(&ObjectMethodPair<C>::Clone)
{}

template <typename ... TArgs>
template <typename C>
inline Slot<TArgs...>::Slot(C* object, void (C::* method)(TArgs...) const)
	: _target(new ObjectConstMethodPair<C>(object, method))
	, _invoke(&InvokeConstMethod<C>)
	, _delete(&ObjectConstMethodPair<C>::Delete)
	, _compare(&ObjectConstMethodPair<C>::Compare)
	, _clone(&ObjectConstMethodPair<C>::Clone)
{}

template <typename ... TArgs>
inline Slot<TArgs...>::~Slot()
{
	if (_delete)
		_delete(_target);
}

template <typename ... TArgs>
void inline Slot<TArgs...>::operator()(TArgs... args)
{
	_invoke(_target, args...);
}

template <typename ... TArgs>
bool inline Slot<TArgs...>::operator==(const Slot<TArgs...>& other)
{
	if (_compare != other._compare)
		return false;
	//Only compare Objects if both Slots use the same CompareFunction
	return _compare(_target, other._target);
}

template <typename ... TArgs>
bool inline Slot<TArgs...>::operator!=(const Slot<TArgs...>& other)
{
	return !operator==(other);
}

template <typename ... TArgs>
inline Slot<TArgs...>& Slot<TArgs...>::operator=(const Slot<TArgs...>& other) 
{
	if(_delete)
		_delete(_target);

	_target  = other._clone(other._target);
	_invoke  = other._invoke;
	_delete  = other._delete;
	_compare = other._compare;
	_clone   = other._clone;

	return *this;
}

template <typename ... TArgs>
static void Slot<TArgs...>::InvokeFunction(void* function, TArgs... args)
{
	static_cast<void(*)(TArgs...)>(function)(args...);
}

template <typename ... TArgs>
template <typename FunctorType>
static void Slot<TArgs...>::InvokeFunctor(void* functor, TArgs... args)
{
	(*static_cast<FunctorType*>(functor))(args...);
}

template <typename ... TArgs>
template <typename C>
static void Slot<TArgs...>::InvokeMethod(void* p, TArgs... args)
{
	auto pair = static_cast<ObjectMethodPair<C>*>(p);
	((pair->_object)->*(pair->_method))(args...);
}

template <typename ... TArgs>
template <typename C>
static void Slot<TArgs...>::InvokeConstMethod(void* p, TArgs... args)
{
	auto pair = static_cast<ObjectConstMethodPair<C>*>(p);
	((pair->_object)->*(pair->_method))(args...);
}


template <typename ... TArgs>
template <typename C>
static void Slot<TArgs...>::ObjectMethodPair<C>::Delete(void* object)
{
	delete static_cast<ObjectMethodPair<C>*>(object);
}

template <typename ... TArgs>
template <typename C>
static bool Slot<TArgs...>::ObjectMethodPair<C>::Compare(void* p1, void* p2)
{
	auto pair1 = static_cast<ObjectMethodPair<C>*>(p1);
	auto pair2 = static_cast<ObjectMethodPair<C>*>(p2);
	return pair1->object == pair2->object 
		&& pair1->method == pair2->method;
}

template <typename ... TArgs>
template <typename C>
static void* Slot<TArgs...>::ObjectMethodPair<C>::Clone(void* p)
{
	auto* pair = static_cast<ObjectMethodPair<C>*>(p);
	return new ObjectMethodPair<C>(pair->object, pair->method);
}

template <typename ... TArgs>
template <typename C>
static void Slot<TArgs...>::ObjectConstMethodPair<C>::Delete(void* object)
{
	delete static_cast<ObjectConstMethodPair<C>*>(object);
}
template <typename ... TArgs>
template <typename C>
static bool Slot<TArgs...>::ObjectConstMethodPair<C>::Compare(void* p1, void* p2)
{
	auto pair1 = static_cast<ObjectConstMethodPair<C>*>(p1);
	auto pair2 = static_cast<ObjectConstMethodPair<C>*>(p2);
	return pair1->object == pair2->object 
		&& pair1->method == pair2->method;
}

template <typename ... TArgs>
template <typename C>
static void* Slot<TArgs...>::ObjectConstMethodPair<C>::Clone(void* p)
{
	auto* pair = static_cast<ObjectConstMethodPair<C>*>(p);
	return new ObjectConstMethodPair<C>(pair->object, pair->method);
}

template <typename ... TArgs>
static bool Slot<TArgs...>::ComparePointerAddress(void* p1, void* p2)
{
	return p1 == p2;
}

template <typename ... TArgs>
static void* Slot<TArgs...>::ClonePointer(void* p)
{
	return p;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/