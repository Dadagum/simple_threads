#ifndef SIMPLE_THREADS_UNIQUE_H
#define SIMPLE_THREADS_UNIQUE_H

#include "simple_thread.h"
#include <pthread.h>

namespace simple_threads {

class UniqueLock {

public:
    UniqueLock(pthread_mutex_t& mtx, bool defer);
    ~UniqueLock();
    int lock();
    int unlock();

private:
    pthread_mutex_t& mtx_;
    static __thread bool locked;
};

}

#endif