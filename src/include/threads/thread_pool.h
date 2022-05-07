#ifndef SIMPLE_THREADS_THREAD_POOL_H
#define SIMPLE_THREADS_THREAD_POOL_H

#include <future>
#include <functional>
#include <utility>
#include <thread>
#include <atomic>
#include <vector>
#include <stdexcept>
#include "utils/pc_queue.h"

#include "simple_thread.h"

namespace simple_threads {

class ThreadPool {
public:
    using Task = std::function<void()>;

    ThreadPool(int threads_num, int tasks_num = 64): pcq(tasks_num), 
    threads_num_(threads_num), stopped(false){}

    /**
     * 初始化线程池, 二步构造，避免 this 指针没有初始化完成
     */
    void init() {
        threads.reserve(threads_num_);
        for (int i = 0; i < threads_num_; ++i) {
            std::thread th(thread_routine, this);
            threads[i] = std::move(th);
        }
    }

    ~ThreadPool() {
        stopped.store(true);
        for (int i = 0; i < threads_num_; ++i) {
            pthread_t th = threads[i].native_handle();
            pthread_cancel(th);
            pthread_join(th, NULL);
        }
    }

    template <typename F, typename... Args>
    auto submit(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type> {
        // 线程池已经关闭
        if (stopped.load()) 
            throw std::runtime_error("Err! Attempt to submit a task to a stopped ThreadPool!");

        // 函数返回类型
        using ret_type = typename std::result_of<F(Args...)>::type;

        // 将传入的函数和参数打包为 packaged_task<R(ArgTypes...)> 
        std::packaged_task<ret_type()> task(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        
        // 将任务放入任务队列
        pcq.emplace([task](){task();});

        return task->get_future();
    }

private:
    static void thread_routine(ThreadPool* pool) {
        while (!pool->stopped.load()) {
            Task task = pool->pcq.pop(); // cancellation point
            task();
        }
    }

    std::vector<std::thread> threads;
    PCQueue<Task> pcq;
    int threads_num_;
    std::atomic<bool> stopped;
};

}

#endif