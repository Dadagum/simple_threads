#ifndef SIMPLE_THREADS_THREAD_POOL_H
#define SIMPLE_THREADS_THREAD_POOL_H

#include <vector>
#include <utility>
#include <pthread.h>
#include "simple_thread.h"
#include "utils/concurrent_queue.h"
#include "utils/semaphore.h"


namespace simple_threads {

using Task = void (*)(void*);
using Args = void*;

class ThreadPool {

public:
    ThreadPool(int threads_num, int task_capacity);
    ~ThreadPool();
    bool submit(Task task, void* args);
    void shutdown(); // 执行所有提交过的 Task 后线程才退出
    void shutdownNow(); // 立刻退出线程

private:
    DISALLOW_COPY_AND_ASSIGN(ThreadPool);
    void start_up_threads();
    std::vector<pthread_t> thread_pool;
    ConcurrentQueue<std::pair<Task, void*>> task_pool;
    int threads_num_;
    int task_capacity_;
    Semaphore empty; // 任务队列空余空间
    Semaphore full; // 任务队列中任务的数量
    bool running;
    pthread_mutex_t mtx; // 保护 running 的互斥量
    pthread_mutex_t shutdown_mtx;
    pthread_cond_t shutdown_cond; // 是否能 cancel 线程的条件

    friend void* thread_routine(void*); // 线程池线程例程
};

}

#endif