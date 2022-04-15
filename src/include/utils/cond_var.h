#ifndef SIMPLE_THREADS_COND_VAR_H
#define SIMPLE_THREADS_COND_VAR_H

#include "simple_thread.h"

namespace simple_threads {

class CondVar {
public:
    CondVar() {
        pthread_cond_init(&cond, NULL);
    }
    ~CondVar() {
        pthread_cond_destroy(&cond);
    }
    int signal() {
        return pthread_cond_signal(&cond);
    }
    int broadcast() {
        return pthread_cond_broadcast(&cond);
    }
    int wait(Mutex* mutex) {
        return pthread_cond_wait(&cond, &(mutex->mtx));
    }

private:
    COPY_CONSTRUCT_ASSIGN(CondVar);
    pthread_cond_t cond;

};

}

#endif