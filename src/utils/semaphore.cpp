#include "utils/semaphore.h"

namespace simple_threads {

Semaphore::Semaphore(int cnt = 1) : cnt_(cnt) {
    if (cnt < 0) cnt_ = 1;
}

Semaphore::~Semaphore() {}

void Semaphore::P() {
    LockGuard lock(mtx);
    while (cnt_ == 0)
        cond.wait(&mtx);
    --cnt_;
}

void Semaphore::V() {
    {
        LockGuard lock(mtx);
        ++cnt_;
    }
    cond.signal();
}

int Semaphore::num() {
    LockGuard lock(mtx);
    return cnt_;
}

}