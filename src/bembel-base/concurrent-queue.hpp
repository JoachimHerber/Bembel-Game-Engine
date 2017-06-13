#ifndef BEMBEL_CONCURRENTQUEUE_H
#define BEMBEL_CONCURRENTQUEUE_H
/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <queue>
#include <mutex>
#include <condition_variable>

/*============================================================================*/
/* CLASS DEFINITIONS                                                          */
/*============================================================================*/
namespace bembel{

template<typename T>
class ConcurrentQueue
{
public:
	ConcurrentQueue();
	~ConcurrentQueue();

	bool IsEmpty() const;

	void Push(const T&);

	bool TryPop(T&);
	T WaitPop();

	void Erase(const T&);

private:
	std::queue<T> _queue;

	mutable std::mutex _mutex;
	std::condition_variable _cv;
};

} //end of namespace bembel
/*============================================================================*/
/* INLINE IMPLEMENTATIONS													  */
/*============================================================================*/

#include "ConcurrentQueue.inl"

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
#endif //include guards
