#ifndef SIMPLE_THREADS_FUTURE_H
#define SIMPLE_THREADS_FUTURE_H

#include "simple_thread.h"
#include "utils/lock_guard.h"
#include "utils/unique_ptr.h"

namespace simple_threads {

template<typename T>
class Future {
public:
    Future() : flag(false), ptr(nullptr) {
        mtx_ptr.set(new pthread_mutex_t);
        cond_ptr.set(new pthread_cond_t);
        pthread_mutex_init(mtx_ptr, NULL);
        pthread_cond_init(cond_ptr, NULL);
    }
    ~Future() {
        pthread_mutex_destroy(mtx_ptr);
        pthread_cond_destroy(cond_ptr);
    }
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
    UniquePtr<pthread_mutex_t> mtx_ptr;
    UniquePtr<pthread_cond_t> cond_ptr;
};

}

#endif