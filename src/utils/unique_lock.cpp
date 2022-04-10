#include "utils/unique_lock.h"

namespace simple_threads {

__thread bool UniqueLock::locked = false;

UniqueLock::UniqueLock(pthread_mutex_t& mtx, bool defer = false) : mtx_(mtx) {
    if (!defer) pthread_mutex_lock(&mtx_);
    locked = !defer;
}

UniqueLock::~UniqueLock() {
    if (locked) pthread_mutex_unlock(&mtx_);
}

int UniqueLock::lock() {
    locked = true;
    return pthread_mutex_lock(&mtx_);
}

int UniqueLock::unlock() {
    locked = false;
    return pthread_mutex_unlock(&mtx_);
}

}