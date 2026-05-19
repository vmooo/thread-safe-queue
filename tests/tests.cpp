#include <gtest/gtest.h>
#include "../include/BoundedQueue.h"

TEST(BoundedQueueTests, test1) {
    BoundedQueue<int, 30> queue;
    queue.try_push(1);
    int ans = queue.try_pop();
    ASSERT_TRUE(ans == 1);
}