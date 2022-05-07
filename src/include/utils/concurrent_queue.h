#ifndef SIMPLE_THREADS_CONCURRENT_QUEUE
#define SIMPLE_THREADS_CONCURRENT_QUEUE

#include <queue>
#include <mutex>
#include "simple_thread.h"

namespace simple_threads {

/**
 * 线程安全队列简单实现
 */
template<typename T>
class ConcurrentQueue {
public:
    ConcurrentQueue(){}

    bool empty() {
        std::lock_guard<std::mutex> guard(mtx);
        return q.empty();
    }

    size_t size() {
        std::lock_guard<std::mutex> guard(mtx);
        return q.size();
    }

    T front() {
        std::lock_guard<std::mutex> guard(mtx);
        return q.front();
    }

    T pop() {
        std::lock_guard<std::mutex> guard(mtx);
        T ft = q.front(); q.pop();
        return ft;
    }

    void push(T item) {
        std::lock_guard<std::mutex> guard(mtx);
        q.push(item);
    }

    void emplace(T &&item) {
        std::lock_guard<std::mutex> guard(mtx);
        q.emplace(std::move(item));
    }

    void clear() {
        std::lock_guard<std::mutex> guard(mtx);
        while (!q.empty()) q.pop();
    }

    DISALLOW_COPY_CONSTRUCT_ASSIGN(ConcurrentQueue) 

private:
    std::queue<T> q;
    std::mutex mtx;
};

}

#endif