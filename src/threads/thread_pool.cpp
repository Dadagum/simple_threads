#include "threads/thread_pool.h"
#include <unistd.h>
#include <stdio.h>

namespace simple_threads {

ThreadPool::ThreadPool(int threads_num, int task_capacity = 64) : 
task_pool(task_capacity), threads_num_(threads_num), task_capacity_(task_capacity), 
empty(task_capacity), full(0), running(true){
    thread_pool.reserve(threads_num_);
    pthread_mutex_init(&mtx, NULL);
    pthread_mutex_init(&shutdown_mtx, NULL);
    pthread_cond_init(&shutdown_cond, NULL);
}

ThreadPool::~ThreadPool() {
    shutdown();
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
        std::pair<Task, Args> task = tptr->task_pool.pop();
        // 完成任务，有待优化，如果 task 时间长，生产者阻塞
        // 但是为了 shutdown() 正确，暂时先包括在信号量中
        task.first(task.second); 
        tptr->empty.V();
        {
            LockGuard lock(tptr->shutdown_mtx);
            if (tptr->empty.num() == tptr->task_capacity_)
                pthread_cond_signal(&tptr->shutdown_cond);
        }
    }
}

void ThreadPool::init() {
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
        while (empty.num() < task_capacity_) 
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