#include "threads/thread_pool.h"

namespace simple_threads {

ThreadPool::ThreadPool(int threads_num, int task_capacity = 64) : 
threads_num_(threads_num), task_pool(task_capacity), task_capacity_(task_capacity), 
running(true), empty(task_capacity), full(0){
    thread_pool.reserve(threads_num_);
    pthread_mutex_init(&mtx, NULL);
    pthread_mutex_init(&shutdown_mtx, NULL);
    pthread_cond_init(&shutdown_cond, NULL);
    start_up_threads(); // 启动线程
}

ThreadPool::~ThreadPool() {
    {
        LockGuard lock(mtx);
        shutdown();
    }
    pthread_mutex_destroy(&mtx);
    pthread_mutex_destroy(&shutdown_mtx);
    pthread_cond_destroy(&shutdown_cond);
}

bool ThreadPool::submit(Task task, void* args) {
    {
        LockGuard lock(mtx);
        if (!running) return false;
    }
    empty.P();
    task_pool.push(std::make_pair(task, args));
    full.V();
    return true;
}

void* thread_routine(void* arg) {
    ThreadPool* tptr = static_cast<ThreadPool*>(arg);
    while (true) {
        tptr->full.P();
        void* (*func)(void*) = NULL;
        void* args = NULL;
        std::pair<Task, Args> task = tptr->task_pool.pop();
        tptr->empty.V();
        task.first(task.second); // do task
        {
            LockGuard lock(tptr->shutdown_mtx);
            if (tptr->full.num() == 0)
                pthread_cond_signal(&tptr->shutdown_cond);
        }
    }
}

void ThreadPool::start_up_threads() {
    for (int i = 0; i < threads_num_; ++i) {
        pthread_create(&thread_pool[i], NULL, thread_routine, this);
    }
}

void ThreadPool::shutdown() {
    {
        LockGuard lock(mtx);
        if (!running) return; // already shutdown
        running = false;
    }
    {
        LockGuard lock(shutdown_mtx);
        while (full.num() > 0) 
            pthread_cond_wait(&shutdown_cond, &shutdown_mtx); // 线程仍然没有完成提交的任务
    }
    shutdownNow();
}

void ThreadPool::shutdownNow() {
    for (int i = 0; i < threads_num_; ++i) {
        pthread_cancel(thread_pool[i]);
    }
}

}