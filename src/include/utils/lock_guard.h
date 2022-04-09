#ifndef SIMPLE_THREADS_LOCK_GUARD_H
#define SIMPLE_THREADS_LOCK_GUARD_H

#include <pthread.h>
#include "simple_thread.h"

namespace simple_threads {
    
class LockGuard {
public:
    LockGuard(pthread_mutex_t& mtx) : mtx_(mtx) {
        pthread_mutex_lock(&mtx_);        
    }
    ~LockGuard() {
        pthread_mutex_unlock(&mtx_);
    }
    
private:
    DISALLOW_COPY_AND_ASSIGN(LockGuard);
    pthread_mutex_t& mtx_;
};

}

#endif