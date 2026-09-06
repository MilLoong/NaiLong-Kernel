/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#include "nk_list"

#include <gtest/gtest.h>

struct MyData {
  int x;
  double y;

  bool operator==(const MyData& other) const {
    return x == other.x && y == other.y;
  }
};

TEST(NkListTest, DefaultConstructor) {
  nk_std::list<int> nk_list;
  EXPECT_TRUE(nk_list.empty());
  EXPECT_EQ(nk_list.size(), 0);
}

TEST(NkListTest, PushFront) {
  nk_std::list<int> nk_list;
  nk_list.push_front(1);
  EXPECT_EQ(nk_list.size(), 1);
  EXPECT_EQ(nk_list.front(), 1);
  EXPECT_EQ(nk_list.back(), 1);

  nk_list.push_front(2);
  EXPECT_EQ(nk_list.size(), 2);
  EXPECT_EQ(nk_list.front(), 2);
  EXPECT_EQ(nk_list.back(), 1);
}

TEST(NkListTest, PushBack) {
  nk_std::list<int> nk_list;
  nk_list.push_back(1);
  EXPECT_EQ(nk_list.size(), 1);
  EXPECT_EQ(nk_list.front(), 1);
  EXPECT_EQ(nk_list.back(), 1);

  nk_list.push_back(2);
  EXPECT_EQ(nk_list.size(), 2);
  EXPECT_EQ(nk_list.front(), 1);
  EXPECT_EQ(nk_list.back(), 2);
}

TEST(NkListTest, PopFront) {
  nk_std::list<int> nk_list;
  nk_list.push_back(1);
  nk_list.push_back(2);
  nk_list.pop_front();
  EXPECT_EQ(nk_list.size(), 1);
  EXPECT_EQ(nk_list.front(), 2);
  nk_list.pop_front();
  EXPECT_EQ(nk_list.size(), 0);
}

TEST(NkListTest, PopBack) {
  nk_std::list<int> nk_list;
  nk_list.push_back(1);
  nk_list.push_back(2);
  nk_list.pop_back();
  EXPECT_EQ(nk_list.size(), 1);
  EXPECT_EQ(nk_list.back(), 1);
  nk_list.pop_back();
  EXPECT_EQ(nk_list.size(), 0);
}

TEST(NkListTest, Insert) {
  nk_std::list<int> nk_list;
  nk_list.push_back(1);
  nk_list.push_back(3);

  auto it = nk_list.begin();
  ++it;
  nk_list.insert(it, 2);

  EXPECT_EQ(nk_list.size(), 3);
  int i = 1;
  for (const auto& item : nk_list) {
    EXPECT_EQ(item, i++);
  }
}

TEST(NkListTest, Erase) {
  nk_std::list<int> nk_list;
  nk_list.push_back(1);
  nk_list.push_back(2);
  nk_list.push_back(3);

  auto it = nk_list.begin();
  ++it;
  nk_list.erase(it);

  EXPECT_EQ(nk_list.size(), 2);
  EXPECT_EQ(nk_list.front(), 1);
  EXPECT_EQ(nk_list.back(), 3);
}

TEST(NkListTest, Clear) {
  nk_std::list<int> nk_list;
  nk_list.push_back(1);
  nk_list.push_back(2);
  nk_list.clear();
  EXPECT_EQ(nk_list.size(), 0);
  EXPECT_TRUE(nk_list.empty());
}

TEST(NkListTest, CopyConstructor) {
  nk_std::list<int> l1;
  l1.push_back(1);
  l1.push_back(2);

  nk_std::list<int> l2(l1);
  EXPECT_EQ(l2.size(), 2);
  EXPECT_EQ(l2.front(), 1);
  EXPECT_EQ(l2.back(), 2);

  l1.pop_back();
  EXPECT_EQ(l1.size(), 1);
  EXPECT_EQ(l2.size(), 2);
}

TEST(NkListTest, AssignmentOperator) {
  nk_std::list<int> l1;
  l1.push_back(1);
  l1.push_back(2);

  nk_std::list<int> l2;
  l2 = l1;
  EXPECT_EQ(l2.size(), 2);
  EXPECT_EQ(l2.front(), 1);
  EXPECT_EQ(l2.back(), 2);

  l1.pop_back();
  EXPECT_EQ(l1.size(), 1);
  EXPECT_EQ(l2.size(), 2);
}

TEST(NkListTest, FloatList) {
  nk_std::list<float> list;
  list.push_back(1.1f);
  list.push_back(2.2f);

  EXPECT_EQ(list.size(), 2);
  EXPECT_FLOAT_EQ(list.front(), 1.1f);
  EXPECT_FLOAT_EQ(list.back(), 2.2f);

  list.pop_front();
  EXPECT_EQ(list.size(), 1);
  EXPECT_FLOAT_EQ(list.front(), 2.2f);
}

TEST(NkListTest, StructList) {
  nk_std::list<MyData> list;
  list.push_back({1, 1.1});
  list.push_back({2, 2.2});

  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list.front().x, 1);
  EXPECT_DOUBLE_EQ(list.front().y, 1.1);
  EXPECT_EQ(list.back().x, 2);
  EXPECT_DOUBLE_EQ(list.back().y, 2.2);

  list.pop_back();
  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(list.front().x, 1);
}

TEST(NkListTest, EraseRange) {
  nk_std::list<int> nk_list;
  for (int i = 1; i <= 5; ++i) {
    nk_list.push_back(i);
  }

  auto first = nk_list.begin();
  ++first;  // points to 2
  auto last = first;
  ++last;
  ++last;
  ++last;  // points to 5

  nk_list.erase(first, last);  // erase 2, 3, 4
  EXPECT_EQ(nk_list.size(), 2);
  EXPECT_EQ(nk_list.front(), 1);
  EXPECT_EQ(nk_list.back(), 5);
}

TEST(NkListTest, Remove) {
  nk_std::list<int> nk_list;
  nk_list.push_back(1);
  nk_list.push_back(2);
  nk_list.push_back(2);
  nk_list.push_back(3);
  nk_list.push_back(2);

  nk_list.remove(2);
  EXPECT_EQ(nk_list.size(), 2);
  EXPECT_EQ(nk_list.front(), 1);
  EXPECT_EQ(nk_list.back(), 3);
}

TEST(NkListTest, RemoveIf) {
  nk_std::list<int> nk_list;
  for (int i = 1; i <= 10; ++i) {
    nk_list.push_back(i);
  }

  nk_list.remove_if([](int x) { return x % 2 == 0; });  // remove even numbers
  EXPECT_EQ(nk_list.size(), 5);

  int expected = 1;
  for (const auto& item : nk_list) {
    EXPECT_EQ(item, expected);
    expected += 2;
  }
}
