#ifndef SIMPLE_THREADS_UNIQUE_PTR_H
#define SIMPLE_THREADS_UNIQUE_PTR_H

#include "simple_thread.h"

namespace simple_threads {

template<typename T>
class UniquePtr {
public:
    typedef void (*Deletor)(T*);

    UniquePtr(T* ptr = nullptr, Deletor deletor = nullptr) : raw_ptr(ptr), deletor_(deletor) {}

    UniquePtr(UniquePtr&& ptr) {
        raw_ptr = ptr.raw_ptr;
        deletor_ = ptr.deletor;
        ptr.raw_ptr = nullptr;
    }

    ~UniquePtr() {
        delete_ptr();
    }

    UniquePtr& operator=(UniquePtr&& ptr) {
        if (*this != ptr) {
            delete_ptr();
            raw_ptr = ptr.raw_ptr;
            deletor_ = ptr.deletor;
        }
    }

    T* release() {
        T* res = raw_ptr;
        raw_ptr = nullptr;
        return res;
    }

    T* get() {
        return raw_ptr;
    }

    void reset(T* ptr = nullptr) {
        delete_ptr();
        raw_ptr = ptr;
    }

    T& operator*() {
        return *raw_ptr;
    }

    T* operator->() {
        return raw_ptr;
    }

private:
    COPY_CONSTRUCT_ASSIGN(UniquePtr);
    void delete_ptr() {
        if (!raw_ptr) return;
        if (deletor_) deletor(raw_ptr);
        else delete raw_ptr;
        raw_ptr = nullptr;
    }
    T* raw_ptr;
    Deletor deletor_;
};

}

#endif
