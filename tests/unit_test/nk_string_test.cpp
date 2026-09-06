/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#include <gtest/gtest.h>

// Rename functions to avoid conflict with standard library
#define memcpy nk_memcpy
#define memmove nk_memmove
#define memset nk_memset
#define memcmp nk_memcmp
#define memchr nk_memchr
#define strcpy nk_strcpy
#define strncpy nk_strncpy
#define strcat nk_strcat
#define strcmp nk_strcmp
#define strncmp nk_strncmp
#define strlen nk_strlen
#define strnlen nk_strnlen
#define strchr nk_strchr
#define strrchr nk_strrchr

// Include the source file directly to test the implementation
// We need to use extern "C" because the included file is C
// but the functions inside are already wrapped in extern "C" in the .h file
// However, the .c file implementation is compiling as C++ here because the test
// file is .cpp The .c file content: #include "nk_string.h" extern "C" {
// ...
// }
// This structure is fine for C++ compilation too.

#include "../../src/libc/nk_string.c"

TEST(NkStringTest, Memcpy) {
  char src[] = "hello";
  char dest[10];
  nk_memcpy(dest, src, 6);
  EXPECT_STREQ(dest, "hello");
}

TEST(NkStringTest, Memmove) {
  char str[] = "memory move test";
  // Overlap: dest > src
  nk_memmove(str + 7, str, 6);  // "memory " -> "memory " at pos 7
  // "memory memoryest"
  EXPECT_STREQ(str, "memory memoryest");

  char str2[] = "memory move test";
  // Overlap: dest < src
  nk_memmove(str2, str2 + 7, 4);  // "move" -> starts at 0
  // "move ry move test"
  EXPECT_EQ(str2[0], 'm');
  EXPECT_EQ(str2[1], 'o');
  EXPECT_EQ(str2[2], 'v');
  EXPECT_EQ(str2[3], 'e');
}

TEST(NkStringTest, Memset) {
  char buffer[10];
  nk_memset(buffer, 'A', 5);
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(buffer[i], 'A');
  }
}

TEST(NkStringTest, Memcmp) {
  char s1[] = "abc";
  char s2[] = "abc";
  char s3[] = "abd";
  char s4[] = "aba";

  EXPECT_EQ(nk_memcmp(s1, s2, 3), 0);
  EXPECT_LT(nk_memcmp(s1, s3, 3), 0);  // 'c' < 'd' -> 99 - 100 < 0
  EXPECT_GT(nk_memcmp(s1, s4, 3), 0);  // 'c' > 'a'
}

TEST(NkStringTest, Memchr) {
  char s[] = "hello world";
  void* res = nk_memchr(s, 'w', 11);
  EXPECT_EQ(static_cast<char*>(res), &s[6]);

  res = nk_memchr(s, 'z', 11);
  EXPECT_EQ(res, nullptr);
}

TEST(NkStringTest, Strcpy) {
  char src[] = "test";
  char dest[10];
  nk_strcpy(dest, src);
  EXPECT_STREQ(dest, "test");
}

TEST(NkStringTest, Strncpy) {
  char src[] = "test string";
  char dest[20];

  // Normal case
  nk_strncpy(dest, src, 4);
  // manual null termination check if expected,
  // but standard strncpy does NOT null terminate if limit reached?
  // nk_string.c implementation should be checked.
  // Usually strncpy pads with nulls if n > src_len, and does NOT null terminate
  // if n <= src_len

  // Let's verify behavior with simpler test first
  nk_memset(dest, 0, 20);
  nk_strncpy(dest, "abc", 5);
  EXPECT_STREQ(dest, "abc");

  nk_strncpy(dest, "abcdef", 3);
  EXPECT_EQ(dest[0], 'a');
  EXPECT_EQ(dest[1], 'b');
  EXPECT_EQ(dest[2], 'c');
  // dest[3] should remain 0 from memset
  EXPECT_EQ(dest[3], '\0');
}

TEST(NkStringTest, Strcat) {
  char dest[20] = "hello";
  nk_strcat(dest, " world");
  EXPECT_STREQ(dest, "hello world");
}

TEST(NkStringTest, Strcmp) {
  EXPECT_EQ(nk_strcmp("abc", "abc"), 0);
  EXPECT_LT(nk_strcmp("abc", "abd"), 0);
  EXPECT_GT(nk_strcmp("abc", "aba"), 0);
  EXPECT_LT(nk_strcmp("abc", "abcd"), 0);
}

TEST(NkStringTest, Strncmp) {
  EXPECT_EQ(nk_strncmp("abc", "abd", 2), 0);  // "ab" vs "ab"
  EXPECT_LT(nk_strncmp("abc", "abd", 3), 0);
}

TEST(NkStringTest, Strlen) {
  EXPECT_EQ(nk_strlen("hello"), 5);
  EXPECT_EQ(nk_strlen(""), 0);
}

TEST(NkStringTest, Strnlen) {
  EXPECT_EQ(nk_strnlen("hello", 10), 5);
  EXPECT_EQ(nk_strnlen("hello", 3), 3);
}

TEST(NkStringTest, Strchr) {
  char s[] = "hello";
  EXPECT_STREQ(nk_strchr(s, 'e'), "ello");
  EXPECT_EQ(nk_strchr(s, 'z'), nullptr);
  EXPECT_STREQ(nk_strchr(s, 'l'), "llo");  // first 'l'
}

TEST(NkStringTest, Strrchr) {
  char s[] = "hello";
  EXPECT_STREQ(nk_strrchr(s, 'l'), "lo");  // last 'l'
  EXPECT_EQ(nk_strrchr(s, 'z'), nullptr);
}

// 边界条件测试
TEST(NkStringTest, MemcpyEdgeCases) {
  char src[] = "test";
  char dest[10];

  // 零长度复制
  nk_memcpy(dest, src, 0);

  // 单字节复制
  nk_memcpy(dest, src, 1);
  EXPECT_EQ(dest[0], 't');
}

TEST(NkStringTest, MemsetEdgeCases) {
  char buffer[10];

  // 零长度设置
  nk_memset(buffer, 'A', 0);

  // 使用 0 填充
  nk_memset(buffer, 0, 5);
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(buffer[i], 0);
  }

  // 使用负值填充 (转换为 unsigned char)
  nk_memset(buffer, -1, 3);
  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(static_cast<unsigned char>(buffer[i]), 255);
  }
}

TEST(NkStringTest, StrcmpEdgeCases) {
  // 空字符串比较
  EXPECT_EQ(nk_strcmp("", ""), 0);
  EXPECT_LT(nk_strcmp("", "a"), 0);
  EXPECT_GT(nk_strcmp("a", ""), 0);

  // 一个字符串是另一个的前缀
  EXPECT_LT(nk_strcmp("abc", "abcd"), 0);
  EXPECT_GT(nk_strcmp("abcd", "abc"), 0);
}

TEST(NkStringTest, StrlenEdgeCases) {
  // 空字符串
  EXPECT_EQ(nk_strlen(""), 0);

  // 只有空字符
  const char null_str[] = {'\0', 'a', 'b', '\0'};
  EXPECT_EQ(nk_strlen(null_str), 0);
}

TEST(NkStringTest, StrnlenEdgeCases) {
  // n 为 0
  EXPECT_EQ(nk_strnlen("hello", 0), 0);

  // n 大于字符串长度
  EXPECT_EQ(nk_strnlen("hi", 100), 2);

  // n 等于字符串长度
  EXPECT_EQ(nk_strnlen("hello", 5), 5);
}

TEST(NkStringTest, StrchrEdgeCases) {
  char s[] = "hello";

  // 查找空字符
  EXPECT_EQ(nk_strchr(s, '\0'), &s[5]);

  // 第一个字符
  EXPECT_EQ(nk_strchr(s, 'h'), s);
}

TEST(NkStringTest, StrrchrEdgeCases) {
  char s[] = "hello";

  // 查找空字符
  EXPECT_EQ(nk_strrchr(s, '\0'), &s[5]);

  // 第一个也是最后一个
  EXPECT_EQ(nk_strrchr(s, 'h'), s);
}

TEST(NkStringTest, MemmoveOverlapForward) {
  // 测试前向重叠: dest > src
  char str[] = "1234567890";
  nk_memmove(str + 3, str, 5);  // "12312345890"
  EXPECT_EQ(str[3], '1');
  EXPECT_EQ(str[4], '2');
  EXPECT_EQ(str[5], '3');
  EXPECT_EQ(str[6], '4');
  EXPECT_EQ(str[7], '5');
}

TEST(NkStringTest, MemmoveOverlapBackward) {
  // 测试后向重叠: dest < src
  char str[] = "1234567890";
  nk_memmove(str, str + 3, 5);  // "4567567890"
  EXPECT_EQ(str[0], '4');
  EXPECT_EQ(str[1], '5');
  EXPECT_EQ(str[2], '6');
  EXPECT_EQ(str[3], '7');
  EXPECT_EQ(str[4], '8');
}

TEST(NkStringTest, MemmoveNoOverlap) {
  // 无重叠
  char src[] = "source";
  char dest[10];
  nk_memmove(dest, src, 7);
  EXPECT_STREQ(dest, "source");
}

TEST(NkStringTest, MemchrNotFound) {
  char s[] = "hello world";
  EXPECT_EQ(nk_memchr(s, 'x', 11), nullptr);
  EXPECT_EQ(nk_memchr(s, 'z', 11), nullptr);
}

TEST(NkStringTest, MemcmpEqual) {
  char s1[] = "test";
  char s2[] = "test";
  EXPECT_EQ(nk_memcmp(s1, s2, 4), 0);
}

TEST(NkStringTest, MemcmpDifferentLengths) {
  char s1[] = "abc";
  char s2[] = "abcd";
  // 只比较前3个字节
  EXPECT_EQ(nk_memcmp(s1, s2, 3), 0);
}

TEST(NkStringTest, StrcatMultiple) {
  char dest[30] = "hello";
  nk_strcat(dest, " ");
  nk_strcat(dest, "world");
  nk_strcat(dest, "!");
  EXPECT_STREQ(dest, "hello world!");
}

TEST(NkStringTest, StrncpyPadding) {
  char dest[10];
  nk_memset(dest, 'X', 10);  // 用 'X' 填充

  // 复制短字符串，应该填充空字符
  nk_strncpy(dest, "ab", 5);
  EXPECT_EQ(dest[0], 'a');
  EXPECT_EQ(dest[1], 'b');
  EXPECT_EQ(dest[2], '\0');
  EXPECT_EQ(dest[3], '\0');
  EXPECT_EQ(dest[4], '\0');
}
