#ifndef __CBLOCKING_QUEUE_H__
#define __CBLOCKING_QUEUE_H__

#include <memory>
#include <queue>
#include <condition_variable>
#include <mutex>

namespace NUtils {
namespace DataStructure {

/**
 * @class BlockingQueue
 * @brief A blocking queue Thread safe
*/
template<typename T>
class BlockingQueue {
private:
    std::queue<T> queue;
    std::mutex mtx;
    std::condition_variable cv;
public:
    BlockingQueue(){}
    void push(T element) {
        std::unique_lock lk(mtx);
        queue.push(element);
        cv.notify_one();
    }
    T pop() {
        std::unique_lock lk(mtx);
        cv.wait(lk, [this]() { return !queue.empty(); });
        T value = queue.front();
        queue.pop();
        return value;
    }
    ~BlockingQueue(){}
};

}
}

#endif