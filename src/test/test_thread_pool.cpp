#include "threads/thread_pool.h"
#include <stdio.h>
#include <cstring> 
#include <sstream>

void* print(void* arg); // 测试线程 routine

void* print(void* arg) {
    int idx = *((int *)arg);
    std::stringstream ss;
    ss << idx;
    std::string id;
    ss >> id;
    std::string str = "hello world from " + id;
    printf("%s\n", str.c_str());
    return NULL;
}

int main(int argc, char* argv[]){
    setvbuf(stdout, NULL, _IONBF, 0); // 禁用 stdio 缓冲，保证输出按顺序
    int num_threads = 10, task_capacity = 20;
    if (argc == 2) num_threads = atoi(argv[1]);
    if (argc == 3) task_capacity = atoi(argv[2]);
    simple_threads::ThreadPool pool(num_threads, task_capacity);
    pool.init();
    return 0;
}