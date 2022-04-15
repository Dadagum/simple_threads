#ifndef SIMPLE_THREADS_FUTURE_H
#define SIMPLE_THREADS_FUTURE_H

#include "simple_thread.h"
#include "utils/mutex.h"
#include "utils/cond_var.h"
#include "utils/lock_guard.h"
#include "utils/unique_ptr.h"

namespace simple_threads {

template<typename T>
class Future {
public:
    Future() : flag(false), ptr(nullptr) {
        mtx_ptr.set(new Mutex);
        cond_ptr.set(new CondVar);
    }
    ~Future() {}
    Future(Future&& f) {
        move_(std::move(f));
    }
    Future& operator=(Future&& f) {
        if (*this != f) {
            move_(std::move(f));
        }
        return *this;
    }
    T get() {
        LockGuard lock(*mtx_ptr.get());
        while (!flag) 
            pthread_cond_wait(cond_ptr.get(), mtx_ptr.get());
        return *ptr;
    }
    friend class ThreadPool;

private:
    COPY_CONSTRUCT_ASSIGN(Future); 
    void move_(Future&& f) {
        flag = f.flag;
        ptr = f.ptr;
        f.ptr.reset();
    }
    void set(T* new_ptr) {
        LockGuard lock(*mtx_ptr.get());
        ptr = new_ptr;
        flag = true;
        pthread_cond_signal(cond_ptr.get());
    }
    bool flag;
    UniquePtr<T> ptr; 
    UniquePtr<Mutex> mtx_ptr;
    UniquePtr<CondVar> cond_ptr;
};

}

#endif