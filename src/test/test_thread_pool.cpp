#include "threads/thread_pool.h"
#include <iostream>
#include <stdio.h>

void* print(void* arg); // 测试线程 routine
static void pthread_test(int nums); // 直接使用 pthread 
static void test_destruct(int nums, simple_threads::ThreadPool &pool); // 测试 ThreadPool 自动销毁
static void test_shutdown(int nums, simple_threads::ThreadPool &pool); // 测试 ThreadPool 的 shutdown() 函数
static void test_shutdownNow(int nums, simple_threads::ThreadPool &pool); // 测试 ThreadPool 的 shutdownNow() 函数

void* print(void* arg) {
    int idx = *((int *)arg);
    std::string str = "hello world from " + std::to_string(idx);
    printf("%s\n", str.c_str());
    return NULL;
}

static void pthread_test(int nums) {
    std::vector<pthread_t> vt(nums);
    for (int i = 0; i < nums; ++i) {
        pthread_create(&vt[i], NULL, print, NULL);
    }
}

static void test_shutdownNow(int nums, simple_threads::ThreadPool &pool) {
    std::vector<int> vt(nums);
    for (int i = 0; i < nums; ++i) vt[i] = i+1;
    for (int i = 0; i < nums; ++i) {
        pool.submit(print, &vt[i]);
    }
    pool.shutdownNow();
    printf("shutdownNow() finish\n");
}

static void test_shutdown(int nums, simple_threads::ThreadPool &pool) {
    std::vector<int> vt(nums);
    for (int i = 0; i < nums; ++i) vt[i] = i+1;
    for (int i = 0; i < nums; ++i) {
        pool.submit(print, &vt[i]);
    }
    pool.shutdown();
    printf("shutdown() finish\n");
}

static void test_destruct(int nums, simple_threads::ThreadPool &pool) {
    std::vector<int> vt(nums);
    for (int i = 0; i < nums; ++i) vt[i] = i+1;
    for (int i = 0; i < nums; ++i) {
        pool.submit(print, &vt[i]);
    }
}

int main(int argc, char* argv[]){
    setvbuf(stdout, NULL, _IONBF, 0); // 禁用 stdio 缓冲，保证输出按顺序
    int num_threads = 10, task_capacity = 20;
    if (argc == 2) num_threads = std::stoi(argv[1]);
    if (argc == 3) task_capacity = std::stoi(argv[2]);
    simple_threads::ThreadPool pool(num_threads, task_capacity);
    pool.init();
    test_shutdown(num_threads, pool);
    return 0;
}

