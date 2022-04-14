#include "threads/thread_pool.h"
#include <unistd.h>
#include <stdio.h>

namespace simple_threads {

ThreadPool::ThreadPool(int threads_num, int task_capacity = 64) : 
task_pool(task_capacity), threads_num_(threads_num), task_capacity_(task_capacity), 
empty(task_capacity), full(0), running(true), todo_task(0){
    thread_pool.reserve(threads_num_);
    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&shutdown_cond, NULL);
}

ThreadPool::~ThreadPool() {
    shutdown();
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&shutdown_cond);
}

bool ThreadPool::submit(Task task, void* args) {
    {
        LockGuard lock(mtx);
        if (!running) return false;
        ++todo_task;
    }
    empty.P();
    task_pool.push(std::make_pair(task, args));
    full.V();
    return true;
}

void* thread_routine(void* arg) {
    ThreadPool* tptr = static_cast<ThreadPool*>(arg);
    while (true) {
        tptr->full.P(); // cancellation point
        std::pair<Task, Args> task = tptr->task_pool.pop();
        tptr->empty.V();
        task.first(task.second); // do task
        {
            LockGuard lock(tptr->mtx);  
            --tptr->todo_task;
            if (tptr->todo_task == 0)
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
        while (todo_task > 0) 
            pthread_cond_wait(&shutdown_cond, &mtx); // 线程仍然没有完成提交的任务
    }
    shutdownNow_();
}

void ThreadPool::shutdownNow() {
    {
        LockGuard lock(mtx);
        if (!running) return; // already shutdown
        running = false;
    }
    shutdownNow_();
}

void ThreadPool::shutdownNow_() {
    for (int i = 0; i < threads_num_; ++i) {
        pthread_cancel(thread_pool[i]);
    }
    for (int i = 0; i < threads_num_; ++i) {
        pthread_join(thread_pool[i], NULL);
    }
}

}