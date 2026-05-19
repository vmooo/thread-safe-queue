#ifndef THREAD_SAFE_QUEUE_BOUNDEDQUEUE_H
#define THREAD_SAFE_QUEUE_BOUNDEDQUEUE_H

#include <condition_variable>
#include <cstdint>
#include <memory>

template<typename T, const size_t maxSize>
class BoundedQueue {
    std::array<T, maxSize> data;
    std::condition_variable cv;
    mutable std::mutex mtx;
    size_t front;
    size_t back;
    bool ready;

public:
    BoundedQueue()
        : front(0),
        back(-1),
        ready(false)
    {}

    void try_push(T value) {
        if (back == -1) { // queue is empty
            std::lock_guard<std::mutex> lock(mtx);
            back = front;
            data[back] = value;
        }
        else {
            std::lock_guard<std::mutex> lock(mtx);
            if (++back == maxSize) {
                back = 0;
            }
            if (back == front) {
                ready = true;
                cv.notify_all();
                throw std::runtime_error("Queue is already full");
            }
            data[back] = value;
        }
        cv.notify_all();
    }

    T try_pop() {
        if (back == -1) {
            {
                std::lock_guard<std::mutex> lock(mtx);
                ready = true;
            }
            cv.notify_all();
            throw std::runtime_error("Queue is empty");
        }
        if (front == back) {
            std::lock_guard<std::mutex> lock(mtx);
            back = -1;
            ready = true;
        }
        else {
            std::lock_guard<std::mutex> lock(mtx);
            if (++front == maxSize) {
                front = 0;
            }
            ready = true;
        }
        cv.notify_all();
        return data[front];
    }
};

#endif //THREAD_SAFE_QUEUE_BOUNDEDQUEUE_H