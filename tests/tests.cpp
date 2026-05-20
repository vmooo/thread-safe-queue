#include <gtest/gtest.h>
#include "../include/BoundedQueue.h"
#include <cstring>
#include <thread>

TEST(BoundedQueueTests, test_try_pop1) {
    BoundedQueue<int, 30> queue;
    queue.try_push(1);
    int ans = queue.try_pop();
    ASSERT_TRUE(ans == 1);
}

TEST(BoundedQueueTests, test_try_pop2) {
    try {
        BoundedQueue<int, 30> queue;
        queue.try_pop();
    }
    catch (std::runtime_error& ex) {
        ASSERT_TRUE(strcmp(ex.what(), "Queue is empty") == 0);
    }
}

TEST(BoundedQueueTests, test_try_pop3) {
    BoundedQueue<int, 30> queue;
    queue.try_push(1);
    queue.try_push(2);
    queue.try_pop();
    ASSERT_TRUE(queue.try_pop() == 2);
}

TEST(BoundedQueueTests, test_try_pop4) {
    BoundedQueue<int, 2> q;
    q.try_push(1);
    q.try_push(2);
    try {
        q.try_push(3);
    }
    catch (std::runtime_error& ex) {
        ASSERT_TRUE(std::strcmp(ex.what(), "Queue is already full") == 0);
    }
}

TEST(BoundedQueueTests, test_push1) {
    BoundedQueue<int, 5> q;
    q.push(1);
    ASSERT_TRUE(q.try_pop() == 1);
}

TEST(BoundedQueueTests, test_push2) {
    BoundedQueue<int, 1> q;
    std::cout << "ща будет push(1)\n";
    q.push(1);
    std::jthread t([&q]() {
        std::this_thread::sleep_for(std::chrono::microseconds(500));
        std::cout << "ща будет pop\n";
        q.try_pop();
    });
    std::cout << "ща будет push(2)\n";
    q.push(2);
    ASSERT_TRUE(q.try_pop() == 2);
}

TEST(BoundedQueueTests, test_pop1) {
    BoundedQueue<int, 1> q;
    q.push(1);
    q.pop();
    ASSERT_TRUE(q.isEmpty());
}

TEST(BoundedQueueTests, test_pop2) {
    BoundedQueue<int, 1> q;
    std::jthread t([&q]() {
        std::this_thread::sleep_for(std::chrono::microseconds(500));
        q.push(1);
    });
    q.pop();
    ASSERT_TRUE(q.isEmpty());
}