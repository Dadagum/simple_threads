#ifndef SIMPLE_THREADS_SHARED_PTR_H
#define SIMPLE_THREADS_SHARED_PTR_H

#include "simple_thread.h"
#include "utils/lock_guard.h"
#include "utils/mutex.h"

namespace simple_threads {

template<typename T>
class SharedPtr {
public:
    typedef void (*Deletor)(T*);

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

    T* operator->() {
        return raw_ptr;
    }

    T& operator*() {
        return *raw_ptr;
    }

private:
    void init(T* ptr) {
        raw_ptr = ptr;
        if (ptr) {
            mtx_ptr = new Mutex;
            ref_ptr = new int(1);
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
            delete mtx_ptr;
        }
    }
    void incr_ref(SharedPtr& rhs) {
        if (!rhs.raw_ptr) return;
        LockGuard lock(*(rhs.mtx_ptr));
        raw_ptr = rhs.raw_ptr;
        deletor_ = rhs.deletor_;
        mtx_ptr = rhs.mtx_ptr;
        ref_ptr = rhs.ref_ptr;
        ++(*ref_ptr);
    }

    T* raw_ptr;
    Deletor deletor_;
    Mutex* mtx_ptr; 
    int* ref_ptr;
    bool is_null; // point to nullptr
};

}

#endif


