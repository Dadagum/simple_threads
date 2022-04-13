#include "utils/semaphore.h"
#include <stdio.h>

namespace simple_threads {

Semaphore::Semaphore(int cnt = 1) : cnt_(cnt) {
    if (cnt < 0) cnt_ = 1;
    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&cond, NULL);
}

Semaphore::~Semaphore() {
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cond);
}

void Semaphore::P() {
    LockGuard lock(mtx);
    while (cnt_ == 0)
        pthread_cond_wait(&cond, &mtx);
    --cnt_;
}

void Semaphore::V() {
    LockGuard lock(mtx);
    ++cnt_;
    pthread_cond_signal(&cond);
}

int Semaphore::num() {
    LockGuard lock(mtx);
    return cnt_;
}

}