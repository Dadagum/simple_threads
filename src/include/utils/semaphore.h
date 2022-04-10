#ifndef SIMPLE_THREADS_SEMAPHORE_H
#define SIMPLE_THREADS_SEMAPHORE_H

#include <pthread.h>
#include "simple_thread.h"
#include "utils/lock_guard.h"

namespace simple_threads {

class Semaphore {
public:
    Semaphore(int cnt);
    ~Semaphore();
    void P();
    void V();

private:
    DISALLOW_COPY_AND_ASSIGN(Semaphore);
    pthread_mutex_t mtx;
    pthread_cond_t cond;
    int cnt_;
};

}

#endif