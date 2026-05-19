#include <gtest/gtest.h>
#include "../include/BoundedQueue.h"
#include <cstring>

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
