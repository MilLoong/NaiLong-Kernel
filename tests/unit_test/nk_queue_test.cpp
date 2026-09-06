/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#include "nk_queue"

#include <gtest/gtest.h>

TEST(NkQueueTest, DefaultConstructor) {
  nk_std::queue<int> q;
  EXPECT_TRUE(q.empty());
  EXPECT_EQ(q.size(), 0);
}

TEST(NkQueueTest, Push) {
  nk_std::queue<int> q;
  q.push(1);
  EXPECT_FALSE(q.empty());
  EXPECT_EQ(q.size(), 1);
  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.back(), 1);

  q.push(2);
  EXPECT_EQ(q.size(), 2);
  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.back(), 2);
}

TEST(NkQueueTest, Pop) {
  nk_std::queue<int> q;
  q.push(1);
  q.push(2);
  q.push(3);

  EXPECT_EQ(q.front(), 1);
  q.pop();
  EXPECT_EQ(q.size(), 2);
  EXPECT_EQ(q.front(), 2);

  q.pop();
  EXPECT_EQ(q.size(), 1);
  EXPECT_EQ(q.front(), 3);

  q.pop();
  EXPECT_TRUE(q.empty());
  EXPECT_EQ(q.size(), 0);
}

TEST(NkQueueTest, CopyConstructor) {
  nk_std::queue<int> q1;
  q1.push(1);
  q1.push(2);

  nk_std::queue<int> q2(q1);
  EXPECT_EQ(q2.size(), 2);
  EXPECT_EQ(q2.front(), 1);
  EXPECT_EQ(q2.back(), 2);

  q1.pop();
  EXPECT_EQ(q1.size(), 1);
  EXPECT_EQ(q2.size(), 2);
}
