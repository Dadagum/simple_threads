#ifndef SIMPLE_THREADS_CONCURRENT_QUEUE
#define SIMPLE_THREADS_CONCURRENT_QUEUE

#include <queue>
#include "simple_thread.h"

namespace simple_threads {

template<typename T>
class ConcurrentQueue {
public:
    ConcurrentQueue(){
    }
    ~ConcurrentQueue() {
    }

    bool empty() {
    }
    bool full() {
    }
    size_t size() {
    }
    T front() {
    }
    T pop() {
    }
    void push(T item) {
    }

private:
    std::queue<T> q;
};

}

#endif