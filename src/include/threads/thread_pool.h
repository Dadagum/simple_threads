#ifndef SIMPLE_THREADS_THREAD_POOL_H
#define SIMPLE_THREADS_THREAD_POOL_H

#include <thread>
#include "simple_thread.h"

class ConcurrentQueue;

namespace simple_threads {

template<typename T>
class ThreadPool {

public:
    ThreadPool(int num); 

    bool Submit();

private:
    
    std::vector<pthread_attr_t> threads;

};

}

#endif