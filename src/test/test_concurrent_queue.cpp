#include "threads/thread_pool.h"
#include <iostream>

int main() {
    simple_threads::ConcurrentQueue<int> q(10);
    int nums = 11;
    for (int i = 0; i < nums; ++i) {
        q.push(i);
    }
    while (!q.empty()) {
        std::cout << q.pop() << std::endl;
    }
    return 0;
}