/*============================================================================*/
/* INLINE IMPLEMENTETION                                                      */
/*============================================================================*/
namespace bembel {
	
template<typename ... TArgs>
inline Delegate<TArgs...>::Delegate(Delegate&& other)
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
inline Delegate<TArgs...>::Delegate(const Delegate& other)
	: _target(other._clone(other._target))
	, _invoke(other._invoke)
	, _delete(other._delete)
	, _compare(other._compare)
	, _clone(other._clone)
{}

template<typename ... TArgs>
inline Delegate<TArgs...>::Delegate(void(*funktion)(TArgs...))
	: _target(funktion)
	, _invoke(&InvokeFunction)
	, _delete(nullptr)
	, _compare(&ComparePointerAddress)
	, _clone(&ClonePointer)
{}

template <typename ... TArgs>
template <typename FunctorType>
inline Delegate<TArgs...>::Delegate(FunctorType* functor)
	: _target(functor)
	, _invoke(&InvokeFunctor<FunctorType>)
	, _delete(nullptr)
	, _compare(&ComparePointerAddress)
	, _clone(&ClonePointer)
{}

template <typename ... TArgs>
template <typename C>
inline Delegate<TArgs...>::Delegate(C* object, void (C::* method)(TArgs...))
	: _target(new ObjectMethodPair<C>(object, method))
	, _invoke(&InvokeMethod<C>)
	, _delete(&ObjectMethodPair<C>::Delete)
	, _compare(&ObjectMethodPair<C>::Compare)
	, _clone(&ObjectMethodPair<C>::Clone)
{}

template <typename ... TArgs>
template <typename C>
inline Delegate<TArgs...>::Delegate(C* object, void (C::* method)(TArgs...) const)
	: _target(new ObjectConstMethodPair<C>(object, method))
	, _invoke(&InvokeConstMethod<C>)
	, _delete(&ObjectConstMethodPair<C>::Delete)
	, _compare(&ObjectConstMethodPair<C>::Compare)
	, _clone(&ObjectConstMethodPair<C>::Clone)
{}

template <typename ... TArgs>
inline Delegate<TArgs...>::~Delegate()
{
	if (_delete)
		_delete(_target);
}

template <typename ... TArgs>
void inline Delegate<TArgs...>::operator()(TArgs... args)
{
	_invoke(_target, args...);
}

template <typename ... TArgs>
bool inline Delegate<TArgs...>::operator==(const Delegate<TArgs...>& other)
{
	if (_compare != other._compare)
		return false;
	//Only compare Objects if both Slots use the same CompareFunction
	return _compare(_target, other._target);
}

template <typename ... TArgs>
bool inline Delegate<TArgs...>::operator!=(const Delegate<TArgs...>& other)
{
	return !operator==(other);
}

template <typename ... TArgs>
inline Delegate<TArgs...>& Delegate<TArgs...>::operator=(const Delegate<TArgs...>& other) 
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
static void Delegate<TArgs...>::InvokeFunction(void* function, TArgs... args)
{
	static_cast<void(*)(TArgs...)>(function)(args...);
}

template <typename ... TArgs>
template <typename FunctorType>
static void Delegate<TArgs...>::InvokeFunctor(void* functor, TArgs... args)
{
	(*static_cast<FunctorType*>(functor))(args...);
}

template <typename ... TArgs>
template <typename C>
static void Delegate<TArgs...>::InvokeMethod(void* p, TArgs... args)
{
	auto pair = static_cast<ObjectMethodPair<C>*>(p);
	((pair->_object)->*(pair->_method))(args...);
}

template <typename ... TArgs>
template <typename C>
static void Delegate<TArgs...>::InvokeConstMethod(void* p, TArgs... args)
{
	auto pair = static_cast<ObjectConstMethodPair<C>*>(p);
	((pair->_object)->*(pair->_method))(args...);
}


template <typename ... TArgs>
template <typename C>
static void Delegate<TArgs...>::ObjectMethodPair<C>::Delete(void* object)
{
	delete static_cast<ObjectMethodPair<C>*>(object);
}

template <typename ... TArgs>
template <typename C>
static bool Delegate<TArgs...>::ObjectMethodPair<C>::Compare(void* p1, void* p2)
{
	auto pair1 = static_cast<ObjectMethodPair<C>*>(p1);
	auto pair2 = static_cast<ObjectMethodPair<C>*>(p2);
	return pair1->object == pair2->object 
		&& pair1->method == pair2->method;
}

template <typename ... TArgs>
template <typename C>
static void* Delegate<TArgs...>::ObjectMethodPair<C>::Clone(void* p)
{
	auto* pair = static_cast<ObjectMethodPair<C>*>(p);
	return new ObjectMethodPair<C>(pair->object, pair->method);
}

template <typename ... TArgs>
template <typename C>
static void Delegate<TArgs...>::ObjectConstMethodPair<C>::Delete(void* object)
{
	delete static_cast<ObjectConstMethodPair<C>*>(object);
}
template <typename ... TArgs>
template <typename C>
static bool Delegate<TArgs...>::ObjectConstMethodPair<C>::Compare(void* p1, void* p2)
{
	auto pair1 = static_cast<ObjectConstMethodPair<C>*>(p1);
	auto pair2 = static_cast<ObjectConstMethodPair<C>*>(p2);
	return pair1->object == pair2->object 
		&& pair1->method == pair2->method;
}

template <typename ... TArgs>
template <typename C>
static void* Delegate<TArgs...>::ObjectConstMethodPair<C>::Clone(void* p)
{
	auto* pair = static_cast<ObjectConstMethodPair<C>*>(p);
	return new ObjectConstMethodPair<C>(pair->object, pair->method);
}

template <typename ... TArgs>
static bool Delegate<TArgs...>::ComparePointerAddress(void* p1, void* p2)
{
	return p1 == p2;
}

template <typename ... TArgs>
static void* Delegate<TArgs...>::ClonePointer(void* p)
{
	return p;
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/