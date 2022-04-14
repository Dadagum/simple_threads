#ifndef SIMPLE_THREADS_SHARED_PTR_H
#define SIMPLE_THREADS_SHARED_PTR_H

#include "simple_thread.h"
#include "utils/lock_guard.h"

namespace simple_threads {

template<typename T>
class SharedPtr {
public:
    using Deletor = void (*)(T*); // 日后可以使用 std::functional

    SharedPtr(T* ptr = nullptr, Deletor deletor = nullptr) : deletor_(deletor) {
        init(ptr);
    }
    ~SharedPtr() {
        decr_ref();
    }
    SharedPtr(SharedPtr& rhs) {
        incr_ref(rhs);
    }

    SharedPtr& operator=(SharedPtr& rhs) {
        if (*this != rhs) {
            decr_ref();
            incr_ref(rhs);
        }
    }

    void reset(T* ptr = nullptr) {
        decr_ref();
        init(ptr);
    }

    T* get() {
        return raw_ptr;
    }

private:
    void init(T* ptr) {
        raw_ptr = ptr;
        if (ptr) {
            mtx_ptr = new pthread_mutex_t;
            ref_ptr = new int(1);
            pthread_mutex_init(mtx_ptr, NULL);
        }
    }

    /** return true if it is the last one **/
    void decr_ref() {
        if (!raw_ptr) return;
        bool last = false;
        {
            LockGuard lock(*mtx_ptr);
            --(*ref_ptr);
            if (*ref_cnt == 0) {
                if (deletor_) deletor_(raw_ptr);
                else delete raw_ptr;
                delete ref_ptr; 
                last = true;
            }
        }
        if (last) {
            pthread_mutex_destroy(mtx_ptr);
            delete mtx_ptr;
        }
    }
    void incr_ref(SharedPtr& rhs) {
        if (!rhs.raw_ptr) return;
        LockGuard lock(*rhs.mtx_ptr);
        raw_ptr = rhs.raw_ptr;
        deletor_ = rhs.deletor_;
        mtx_ptr = rhs.mtx_ptr;
        ref_ptr = rhs.ref_ptr;
        ++(*ref_ptr);
    }

    T* raw_ptr;
    Deletor deletor_;
    pthread_mutex_t* mtx_ptr; 
    int* ref_ptr;
    bool is_null; // point to nullptr
};

}

#endif


