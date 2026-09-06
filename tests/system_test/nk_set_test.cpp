/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#include "nk_set"

#include <cstdint>

#include "nk_cstdio"
#include "nk_cstring"
#include "nk_iostream"
#include "system_test.h"

using namespace nk_std;

namespace {

bool test_insert() {
  set<int> s;
  EXPECT_EQ(s.size(), 0, "Initial size should be 0");
  EXPECT_TRUE(s.empty(), "Initial set should be empty");

  s.insert(10);
  EXPECT_EQ(s.size(), 1, "Size should be 1 after inserting 10");
  EXPECT_TRUE(s.find(10) != s.end(), "Should find 10");

  s.insert(5);
  EXPECT_EQ(s.size(), 2, "Size should be 2 after inserting 5");
  EXPECT_TRUE(s.find(5) != s.end(), "Should find 5");

  s.insert(10);  // Duplicate
  EXPECT_EQ(s.size(), 2, "Size should be 2 after inserting duplicate 10");

  return true;
}

bool test_erase() {
  set<int> s;
  s.insert(10);
  s.insert(5);
  s.insert(20);

  EXPECT_EQ(s.size(), 3, "Size should be 3");

  s.erase(5);
  EXPECT_EQ(s.size(), 2, "Size should be 2 after erase(5)");
  EXPECT_TRUE(s.find(5) == s.end(), "Should not find 5");

  s.erase(100);  // Not found
  EXPECT_EQ(s.size(), 2, "Size should be 2 after erase(100)");

  s.erase(10);
  EXPECT_EQ(s.size(), 1, "Size should be 1 after erase(10)");

  s.erase(20);
  EXPECT_EQ(s.size(), 0, "Size should be 0 after erase(20)");
  EXPECT_TRUE(s.empty(), "Set should be empty");

  return true;
}

bool test_iterator() {
  set<int> s;
  s.insert(2);
  s.insert(1);
  s.insert(3);

  auto it = s.begin();
  EXPECT_EQ(*it, 1, "First element should be 1");
  ++it;
  EXPECT_EQ(*it, 2, "Second element should be 2");
  it++;
  EXPECT_EQ(*it, 3, "Third element should be 3");
  ++it;
  EXPECT_TRUE(it == s.end(), "Iterator should be end");

  return true;
}

}  // namespace

auto nk_set_test() -> bool {
  nk_printf("  test_insert...\n");
  if (!test_insert()) {
    return false;
  }
  nk_printf("  test_erase...\n");
  if (!test_erase()) {
    return false;
  }
  nk_printf("  test_iterator...\n");
  if (!test_iterator()) {
    return false;
  }
  return true;
}
