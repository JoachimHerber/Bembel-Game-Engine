namespace bembel::base {

template <typename T>
inline ConcurrentQueue<T>::ConcurrentQueue() {
}

template <typename T>
inline ConcurrentQueue<T>::~ConcurrentQueue() {
}

template <typename T>
inline bool ConcurrentQueue<T>::isEmpty() const {
    std::lock_guard<std::mutex> lock(this->mutex);
    return this->queue.empty();
}

template <typename T>
inline void ConcurrentQueue<T>::push(const T& value) {
    std::unique_lock<std::mutex> lock(this->mutex);
    this->queue.push_back(value);
    lock.unlock();

    this->cv.notify_one();
}

template <typename T>
inline bool ConcurrentQueue<T>::tryPop(T& value) {
    std::lock_guard<std::mutex> lock(this->mutex);
    if(this->queue.empty()) return false;

    value = this->queue.front();
    this->queue.pop_front();
    return true;
}

template <typename T>
inline T ConcurrentQueue<T>::waitPop() {
    std::unique_lock<std::mutex> lock(this->mutex);
    while(this->queue.empty()) this->cv.wait(lock);

    T value = this->queue.front();
    this->queue.pop_front();

    return value;
}

template <typename T>
inline void ConcurrentQueue<T>::erase(const T& value) {
    std::unique_lock<std::mutex> lock(this->mutex);
    for(auto it = this->queue.begin(); it != this->queue.end(); ++it) {
        if(*it == value) {
            this->queue.erase(it);
            return;
        }
    }
}

} // end of namespace bembel::base
