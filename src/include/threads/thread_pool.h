#ifndef SIMPLE_THREADS_THREAD_POOL_H
#define SIMPLE_THREADS_THREAD_POOL_H

#include <future>
#include <functional>
#include <utility>

#include "simple_thread.h"

namespace simple_threads {

class ThreadPool {

public:
    ThreadPool(int threads_num, int task_capacity); 
    void init(); // 初始化线程池
    ~ThreadPool();
    template <typename F, typename... Args>
    auto submit(F &&f, Args &&...args) -> std::future<decltype(std::forward<F>(f)(std::forward<Args>(args)…))>; // 提交一个任务线程
    void shutdown(); // 执行所有提交过的 Task 后线程才退出
    void shutdownNow(); // 立刻退出线程

private:
    

};

}

#endif