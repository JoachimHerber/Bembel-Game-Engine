module;
#include <mutex>
#include <queue>
export module bembel.base:ConcurrentQueue;

import :Types;

export namespace bembel::base {

template <typename T>
class ConcurrentQueue {
    using Lock = std::unique_lock<std::mutex>;

  public:
    ConcurrentQueue()  = default;
    ~ConcurrentQueue() = default;

    bool empty() const {
        Lock lock(m_mutex);
        return m_queue.empty();
    }

    void push(In<T> value) {
        Lock lock(m_mutex);
        m_queue.push_back(value);
        lock.unlock();

        m_cv.notify_one();
    }

    bool tryPop(Out<T> value) {
        Lock lock(m_mutex);
        if(m_queue.empty()) return false;

        value = m_queue.front();
        m_queue.pop_front();
        return true;
    }
    T waitPop() {
        Lock lock(m_mutex);
        while(m_queue.empty()) m_cv.wait(lock);

        T value = m_queue.front();
        m_queue.pop_front();

        return value;
    }

    void erase(In<T> value) {
        Lock lock(m_mutex);
        for(auto it = m_queue.begin(); it != m_queue.end(); ++it) {
            if(*it == value) {
                m_queue.erase(it);
                return;
            }
        }
    }

  private:
    std::queue<T>           m_queue;
    mutable std::mutex      m_mutex;
    std::condition_variable m_cv;
};

} // end of namespace bembel::base
