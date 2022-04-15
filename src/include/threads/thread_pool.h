#ifndef SIMPLE_THREADS_THREAD_POOL_H
#define SIMPLE_THREADS_THREAD_POOL_H

#include <vector>
#include <utility>
#include <pthread.h>
#include "simple_thread.h"
#include "utils/concurrent_queue.h"
#include "utils/semaphore.h"
#include "utils/mutex.h"
#include "utils/cond_var.h"


namespace simple_threads {

using Task = void* (*)(void*);
using Args = void*;

class ThreadPool {

public:
    ThreadPool(int threads_num, int task_capacity);
    void init();
    ~ThreadPool();
    bool submit(Task task, void* args);
    void shutdown(); // 执行所有提交过的 Task 后线程才退出
    void shutdownNow(); // 立刻退出线程

private:
    static void* thread_routine(void*); // 线程池线程例程
    void shutdownNow_();
    COPY_CONSTRUCT_ASSIGN(ThreadPool);
    std::vector<pthread_t> thread_pool;
    ConcurrentQueue<std::pair<Task, void*>> task_pool;
    int threads_num_;
    int task_capacity_;
    Semaphore empty; // 任务队列空余空间
    Semaphore full; // 任务队列中任务的数量
    bool running;
    int todo_task;
    Mutex mtx;
    CondVar shutdown_cond; // 是否能 cancel 线程的条件

};

}

#endif