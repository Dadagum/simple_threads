#ifndef SIMPLE_THREADS_THREAD_POOL_H
#define SIMPLE_THREADS_THREAD_POOL_H

#include <future>
#include <functional>
#include <utility>
#include <thread>
#include <atomic>
#include <vector>
#include <stdexcept>
#include <memory>
#include "utils/pc_queue.h"

#include "simple_thread.h"

namespace simple_threads {

class ThreadPool {
public:
    using Task = std::function<void()>;

    ThreadPool(int threads_num): threads_num_(threads_num), stopped(false){}

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
        cv.notify_all();
        for (int i = 0; i < threads_num_; ++i) {
            if (threads[i].joinable())
                threads[i].join();
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
        auto task = std::make_shared<std::packaged_task<ret_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        
        // 将任务放入任务队列, 唤醒一个工作线程
        q.emplace([task](){(*task)();});
        cv.notify_one();

        return task->get_future();
    }

private:
    static void thread_routine(ThreadPool* pool) {
        Task task;
        while (true) {
            {
                std::unique_lock<std::mutex> lk(pool->mtx);
                pool->cv.wait(lk, [pool](){return pool->stopped.load() || !pool->q.empty();});
                if (pool->stopped.load() && pool->q.empty()) return; 
                task = std::move(pool->q.pop()); 
            }
            task();
        }
    }
    ConcurrentQueue<Task> q;
    int threads_num_;
    std::condition_variable cv;
    std::mutex mtx;
    std::vector<std::thread> threads;
    std::atomic<bool> stopped;
};

}

#endif