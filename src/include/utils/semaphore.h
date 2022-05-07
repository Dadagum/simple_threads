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
        --cnt_;
        cv.wait(lk, [this]{return cnt_ >= 0; });
    }

    void V() {
        {
            std::unique_lock<std::mutex> lk(mtx);
            ++cnt_;
        }
        cv.notify_one();
    }

    void reset(int cnt) {
        {
            std::unique_lock<std::mutex> lk(mtx);
            cnt_ = cnt;
        }
        cv.notify_all();
    }

    DISALLOW_COPY_CONSTRUCT_ASSIGN(Semaphore)

private:
    std::mutex mtx;
    std::condition_variable cv;
    int cnt_;
};

}