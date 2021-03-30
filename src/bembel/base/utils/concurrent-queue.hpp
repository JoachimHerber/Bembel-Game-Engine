#ifndef BEMBEL_BASE_UTILS_CONCURRENTQUEUE_HPP
#define BEMBEL_BASE_UTILS_CONCURRENTQUEUE_HPP

#include <condition_variable>
#include <mutex>
#include <queue>

namespace bembel::base {

template <typename T>
class ConcurrentQueue {
  public:
    ConcurrentQueue();
    ~ConcurrentQueue();

    bool isEmpty() const;

    void push(const T&);

    bool tryPop(T&);
    T waitPop();

    void erase(const T&);

  private:
    std::queue<T> queue;

    mutable std::mutex mutex;
    std::condition_variable cv;
};

} // end of namespace bembel::base

// INLINE IMPLEMENTATIONS
#include "concurrent-queue.inl"

#endif // include guards
