#include "simple_thread.h"
#include "semaphore.h"
#include "concurrent_queue.h"
#include <atomic>

namespace simple_threads {

/**
 *  生产者消费者模式队列(P for producer, C for consumer.)
 */
template<typename T>
class PCQueue {
public:
    PCQueue(int size = 64): full_(0), empty_(size), cancel(false) {
        size_ = size;
    }

    void push(T item) {
        empty_.P();
        cq.push(item);
        full_.V();
    }

    void emplace(T &&item) {
        empty_.P();
        cq.emplace(std::move(item));
        full_.V();
    }

    T pop() {
        full_.P();
        T item = cq.pop();
        empty_.V();
        return item;
    }

    DISALLOW_COPY_CONSTRUCT_ASSIGN(PCQueue)

private:
    int size_; // 限制队列的容量
    Semaphore full_;
    Semaphore empty_;
    ConcurrentQueue<T> cq;
    std::atomic_bool cancel;
};

}