#include "simple_thread.h"
#include <mutex>
#include <condition_variable>

namespace simple_threads {

/**
 * 信号量简单实现
 */
class Semaphore {
public:
    Semaphore(int cnt) {
        cnt_ = cnt;
    }

    void P() {
        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk, [this]{return cnt_ > 0; });
        --cnt_;
    }

    void V() {
        std::unique_lock<std::mutex> lk(mtx);
        ++cnt_;
        cv.notify_one();
    }

    DISALLOW_COPY_CONSTRUCT_ASSIGN(Semaphore)

private:
    std::mutex mtx;
    std::condition_variable cv;
    int cnt_;
};

}