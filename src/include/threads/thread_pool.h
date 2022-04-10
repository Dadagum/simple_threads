#ifndef SIMPLE_THREADS_THREAD_POOL_H
#define SIMPLE_THREADS_THREAD_POOL_H

#include <vector>
#include <pthread.h>
#include "simple_thread.h"
#include "utils/concurrent_queue.h"


namespace simple_threads {

using Task = void (*)(void*);

class ThreadPool {

public:
    ThreadPool(int threads_num, int task_capacity = 64);
    ~ThreadPool();
    bool submit(Task task, void* arg);
    void shutdown();
    void shutdownNow();

private:
    DISALLOW_COPY_AND_ASSIGN(ThreadPool);
    std::vector<pthread_attr_t> thread_pool;
    ConcurrentQueue<Task> task_pool;
    bool isShutdown;
    int threads_num_;
    int task_capacity_;
};

}

#endif