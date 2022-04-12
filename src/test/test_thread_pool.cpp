#include "threads/thread_pool.h"
#include <iostream>
#include <stdio.h>

void* print(void* arg) {
    char* str = "hello world";
    printf("%s\n", str);
    return NULL;
}

void test(int nums) {
    std::vector<pthread_t> vt(nums);
    for (int i = 0; i < nums; ++i) {
        pthread_create(&vt[i], NULL, print, NULL);
    }
    sleep(1);
}

int main(){
    int nums = 5;
   // test(nums);
    simple_threads::ThreadPool pool(nums, 10);
    pool.init();
    int i;
    for (i = 0; i < nums; ++i) {
        pool.submit(print, NULL);
    }
    return 0;
}

