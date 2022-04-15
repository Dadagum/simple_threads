#ifndef SIMPLE_THREADS_MUTEX_H
#define SIMPLE_THREADS_MUTEX_H

#include "simple_thread.h"

namespace simple_threads {

class Condvar;

class Mutex {
public:
    Mutex() {
        pthread_mutex_init(&mtx, NULL);
    }
    ~Mutex() {
        pthread_mutex_destroy(&mtx);
    }
    int lock() {
        return pthread_mutex_lock(&mtx);   
    }
    int unlock() {
        return pthread_mutex_unlock(&mtx);
    }
private:
    friend class CondVar;
    COPY_CONSTRUCT_ASSIGN(Mutex);
    pthread_mutex_t mtx;
};

}

#endif
