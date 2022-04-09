#ifndef SIMPLE_THREADS_CONCURRENT_QUEUE
#define SIMPLE_THREADS_CONCURRENT_QUEUE

#include "lock_guard.h"
#include "simple_thread.h"

namespace simple_threads {

template<typename T>
class ConcurrentQueue {
public:
    ConcurrentQueue(int capacity) : size_(0), capacity_(capacity), start(-1), end(-1) {
        arr = new T[capacity];
        pthread_mutex_init(&mtx);
    }
    ~ConcurrentQueue() {
        delete [] arr;
        pthread_mutex_destroy(&mtx);
    }

    bool empty() const {
        LockGuard(&mtx);
        return size_ == 0;
    }
    bool full() const {
        LockGuard(&mtx);
        return size_ == capacity_;
    }
    size_t size() const {
        LockGuard(&mtx);
        return size_;
    }
    T front() const {
        LockGuard(&mtx);
        return arr[start+1];
    }
    T pop() const {
        LockGuard(&mtx);
        start = (start + 1) % capacity__;
        T res = arr[start];
        --size_;
        rturn res;        
    }
    void push(T item) const {
        LockGuard(&mtx);
        end = (end + 1) % capacity_;
        arr[end] = item;
        ++size_;
    }

private:
    DISALLOW_COPY_AND_ASSIGN(ConcurrentQueue);
    size_t size_;
    size_t capacity_;
    T* arr;
    size_t start;
    size_t end;
    pthread_mutex_t mtx;
};

}

#endif