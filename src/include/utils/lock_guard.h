#ifndef SIMPLE_THREADS_LOCK_GUARD_H
#define SIMPLE_THREADS_LOCK_GUARD_H

#include "simple_thread.h"
#include "utils/mutex.h"

namespace simple_threads {
    
class LockGuard {
public:
    LockGuard(Mutex& mtx) : mtx_(mtx) {
        mtx_.lock();
    }
    ~LockGuard() {
        mtx_.unlock();
    }
    
private:
    COPY_CONSTRUCT_ASSIGN(LockGuard);
    Mutex& mtx_;
};

}

#endif