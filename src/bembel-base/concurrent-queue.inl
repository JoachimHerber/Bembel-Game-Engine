/*============================================================================*/
/* INLINE IMPLEMENTETION                                                      */
/*============================================================================*/
namespace bembel{

template<typename T>
inline ConcurrentQueue<T>::ConcurrentQueue()
{}

template<typename T>
inline ConcurrentQueue<T>::~ConcurrentQueue()
{}

template<typename T>
inline bool ConcurrentQueue<T>::IsEmpty() const
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _queue.empty();
}

template<typename T>
inline void ConcurrentQueue<T>::Push(const T& value)
{
	std::unique_lock<std::mutex> lock(_mutex);
	_queue.push_back(value);
	lock.unlock();

	_cv.notify_one();
}

template<typename T>
inline bool ConcurrentQueue<T>::TryPop(T& value)
{
	std::lock_guard<std::mutex> lock(_mutex);
	if(_queue.empty())
		return false;

	value = _queue.front();
	_queue.pop_front();
	return true;
}

template<typename T>
inline T ConcurrentQueue<T>::WaitPop()
{
	std::unique_lock<std::mutex> lock(_mutex);
	while(_queue.empty())
		_cv.wait(lock);

	T value = _queue.front();
	_queue.pop_front();

	return value;
}

template<typename T>
inline void ConcurrentQueue<T>::Erase(const T& value)
{
	std::unique_lock<std::mutex> lock(_mutex);
	for(auto it = _queue.begin(); it != _queue.end(); ++it)
	{
		if(*it ==  value)
		{
			_queue.erase(it);
			return;
		}
	}
}

} //end of namespace bembel
/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/