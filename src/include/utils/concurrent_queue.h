#ifndef SIMPLE_THREADS_CONCURRENT_QUEUE
#define SIMPLE_THREADS_CONCURRENT_QUEUE

#include "utils/lock_guard.h"
#include "simple_thread.h"

namespace simple_threads {

template<typename T>
class ConcurrentQueue {
public:
    ConcurrentQueue(int capacity) : size_(0), capacity_(capacity), start(-1), end(-1) {
        arr = new T[capacity];
        pthread_mutex_init(&mtx, NULL);
    }
    ~ConcurrentQueue() {
        delete [] arr;
        pthread_mutex_destroy(&mtx);
    }

    bool empty() {
        LockGuard lock(mtx);
        return size_ == 0;
    }
    bool full() {
        LockGuard lock(mtx);
        return size_ == capacity_;
    }
    size_t size() {
        LockGuard lock(mtx);
        return size_;
    }
    T front() {
        LockGuard lock(mtx);
        return arr[end+1];
    }
    T pop() {
        LockGuard lock(mtx);
        end = (end + 1) % capacity_;
        T res = arr[end];
        --size_;
        return res;        
    }
    void push(T item) {
        LockGuard lock(mtx);
        start = (start + 1) % capacity_;
        arr[start] = item;
        ++size_;
    }

private:
    COPY_CONSTRUCT_ASSIGN(ConcurrentQueue);
    size_t size_;
    size_t capacity_;
    T* arr;
    int start;
    int end;
    pthread_mutex_t mtx;
};

}

#endif