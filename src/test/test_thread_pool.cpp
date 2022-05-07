#include "threads/thread_pool.h"
#include <stdio.h>
#include <cstring> 
#include <sstream>
#include <vector>


void* print(int idx) {
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
    int num_threads = 10;
    if (argc == 2) num_threads = atoi(argv[1]);
    simple_threads::ThreadPool pool(5);
    pool.init();
    sleep(1);
    for (int i = 0; i < num_threads; ++i) {
        pool.submit(print, i);
    }
    sleep(5);
    return 0;
}