#ifndef SIMPLE_THREADS_SEMAPHORE_H
#define SIMPLE_THREADS_SEMAPHORE_H

#include "simple_thread.h"
#include "utils/lock_guard.h"

namespace simple_threads {

class Semaphore {
public:
    Semaphore(int cnt);
    ~Semaphore();
    void P();
    void V();
    int num();

private:
    COPY_CONSTRUCT_ASSIGN(Semaphore);
    pthread_mutex_t mtx;
    pthread_cond_t cond;
    int cnt_;
};

}

#endif