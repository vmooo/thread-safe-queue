#ifndef THREAD_SAFE_QUEUE_BOUNDEDQUEUE_H
#define THREAD_SAFE_QUEUE_BOUNDEDQUEUE_H

#include <condition_variable>
#include <cstdint>
#include <memory>
#include <climits>
#include <iostream>

template<typename T, const size_t maxSize>
class BoundedQueue {
    std::array<T, maxSize> data;
    std::condition_variable cv_for_pop;
    std::condition_variable cv_for_push;
    mutable std::mutex mtx;
    size_t front;
    size_t back;
    size_t numberOfElements;
    bool ready_for_pop;
    bool ready_for_push;

    void push_when_empty(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        back = front;
        data[back] = value;
        ++numberOfElements;
        ready_for_pop = true;
    }

public:
    BoundedQueue()
        : front(0),
        back(SIZE_T_MAX),
        numberOfElements(0),
        ready_for_pop(false),
        ready_for_push()
    {}

    bool is_empty() const {
        return numberOfElements == 0;
    }

    bool is_full() const {
        return numberOfElements == maxSize;
    }

    void try_push(const T& value) {
        if (is_empty()) { // queue is empty
            push_when_empty(value);
        }
        else {
            std::lock_guard<std::mutex> lock(mtx);
            if (++back == maxSize) {
                back = 0;
            }
            if (back == front) {
                ready_for_pop = true;
                cv_for_pop.notify_all();
                throw std::runtime_error("Queue is already full");
            }
            ++numberOfElements;
            data[back] = value;
            ready_for_pop = true;
        }
        cv_for_pop.notify_all();
    }

    T try_pop() {
        if (is_empty()) {
            throw std::runtime_error("Queue is empty");
        }
        if (numberOfElements == 1) {
            std::lock_guard<std::mutex> lock(mtx);
            back = SIZE_T_MAX;
            --numberOfElements;
            ready_for_push = true;
        }
        else {
            std::lock_guard<std::mutex> lock(mtx);
            if (++front == maxSize) {
                front = 0;
            }
            --numberOfElements;
            ready_for_push = true;
        }
        cv_for_push.notify_all();
        return data[front];
    }

    void push(const T& value) {
        if (is_full()) {
            std::unique_lock lock(mtx);
            ready_for_push = false;
            cv_for_push.wait(lock, [this]() {
                return ready_for_push;
            });
        }
        if (is_empty()) {
            push_when_empty(value);
        }
        else {
            {
                std::lock_guard<std::mutex> lock(mtx);
                ++back;
            }
            if (back == maxSize) {
                std::lock_guard<std::mutex> lock(mtx);
                back = 0;
            }
            std::lock_guard<std::mutex> lock(mtx);
            ++numberOfElements;
            data[back] = value;
            ready_for_pop = true;
        }
        cv_for_pop.notify_all();
    }

    T pop() {
        if (is_empty()) {
            std::unique_lock lock(mtx);
            ready_for_pop = false;
            cv_for_pop.wait(lock, [this]() {
               return ready_for_pop;
            });
        }
        if (numberOfElements == 1) {
            std::lock_guard<std::mutex> lock(mtx);
            back = SIZE_T_MAX;
            --numberOfElements;
            ready_for_push = true;
        }
        else {
            std::lock_guard<std::mutex> lock(mtx);
            if (++front == maxSize) {
                front = 0;
            }
            --numberOfElements;
            ready_for_push = true;
        }
        cv_for_push.notify_all();
        return data[front];
    }

    T get_front() {
        return data[front];
    }
};

#endif //THREAD_SAFE_QUEUE_BOUNDEDQUEUE_H