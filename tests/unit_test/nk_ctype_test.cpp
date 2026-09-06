/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#include <gtest/gtest.h>

// Rename functions to avoid conflict with standard library
#define isalnum nk_isalnum
#define isalpha nk_isalpha
#define isblank nk_isblank
#define iscntrl nk_iscntrl
#define isdigit nk_isdigit
#define isgraph nk_isgraph
#define islower nk_islower
#define isprint nk_isprint
#define ispunct nk_ispunct
#define isspace nk_isspace
#define isupper nk_isupper
#define isxdigit nk_isxdigit
#define tolower nk_tolower
#define toupper nk_toupper

// Include the source file directly to test the implementation
#include "../../src/libc/nk_ctype.c"

TEST(NkCtypeTest, IsAlnum) {
  EXPECT_TRUE(nk_isalnum('a'));
  EXPECT_TRUE(nk_isalnum('Z'));
  EXPECT_TRUE(nk_isalnum('0'));
  EXPECT_TRUE(nk_isalnum('9'));
  EXPECT_FALSE(nk_isalnum('!'));
  EXPECT_FALSE(nk_isalnum(' '));
}

TEST(NkCtypeTest, IsAlpha) {
  EXPECT_TRUE(nk_isalpha('a'));
  EXPECT_TRUE(nk_isalpha('Z'));
  EXPECT_FALSE(nk_isalpha('0'));
  EXPECT_FALSE(nk_isalpha('!'));
}

TEST(NkCtypeTest, IsBlank) {
  EXPECT_TRUE(nk_isblank(' '));
  EXPECT_TRUE(nk_isblank('\t'));
  EXPECT_FALSE(nk_isblank('\n'));
  EXPECT_FALSE(nk_isblank('a'));
}

TEST(NkCtypeTest, IsCntrl) {
  EXPECT_TRUE(nk_iscntrl(0));
  EXPECT_TRUE(nk_iscntrl(31));
  EXPECT_TRUE(nk_iscntrl(127));
  EXPECT_FALSE(nk_iscntrl(' '));
  EXPECT_FALSE(nk_iscntrl('a'));
}

TEST(NkCtypeTest, IsDigit) {
  EXPECT_TRUE(nk_isdigit('0'));
  EXPECT_TRUE(nk_isdigit('9'));
  EXPECT_FALSE(nk_isdigit('a'));
  EXPECT_FALSE(nk_isdigit(' '));
}

TEST(NkCtypeTest, IsGraph) {
  EXPECT_TRUE(nk_isgraph('!'));
  EXPECT_TRUE(nk_isgraph('a'));
  EXPECT_TRUE(nk_isgraph('~'));
  EXPECT_FALSE(nk_isgraph(' '));
  EXPECT_FALSE(nk_isgraph('\n'));
}

TEST(NkCtypeTest, IsLower) {
  EXPECT_TRUE(nk_islower('a'));
  EXPECT_TRUE(nk_islower('z'));
  EXPECT_FALSE(nk_islower('A'));
  EXPECT_FALSE(nk_islower('0'));
}

TEST(NkCtypeTest, IsPrint) {
  EXPECT_TRUE(nk_isprint(' '));
  EXPECT_TRUE(nk_isprint('a'));
  EXPECT_TRUE(nk_isprint('~'));
  EXPECT_FALSE(nk_isprint('\t'));
  EXPECT_FALSE(nk_isprint(31));
}

TEST(NkCtypeTest, IsPunct) {
  EXPECT_TRUE(nk_ispunct('!'));
  EXPECT_TRUE(nk_ispunct('.'));
  EXPECT_FALSE(nk_ispunct('a'));
  EXPECT_FALSE(nk_ispunct('0'));
  EXPECT_FALSE(nk_ispunct(' '));
}

TEST(NkCtypeTest, IsSpace) {
  EXPECT_TRUE(nk_isspace(' '));
  EXPECT_TRUE(nk_isspace('\f'));
  EXPECT_TRUE(nk_isspace('\n'));
  EXPECT_TRUE(nk_isspace('\r'));
  EXPECT_TRUE(nk_isspace('\t'));
  EXPECT_TRUE(nk_isspace('\v'));
  EXPECT_FALSE(nk_isspace('a'));
}

TEST(NkCtypeTest, IsUpper) {
  EXPECT_TRUE(nk_isupper('A'));
  EXPECT_TRUE(nk_isupper('Z'));
  EXPECT_FALSE(nk_isupper('a'));
  EXPECT_FALSE(nk_isupper('0'));
}

TEST(NkCtypeTest, IsXdigit) {
  EXPECT_TRUE(nk_isxdigit('0'));
  EXPECT_TRUE(nk_isxdigit('9'));
  EXPECT_TRUE(nk_isxdigit('a'));
  EXPECT_TRUE(nk_isxdigit('f'));
  EXPECT_TRUE(nk_isxdigit('A'));
  EXPECT_TRUE(nk_isxdigit('F'));
  EXPECT_FALSE(nk_isxdigit('g'));
  EXPECT_FALSE(nk_isxdigit('G'));
}

TEST(NkCtypeTest, ToLower) {
  EXPECT_EQ(nk_tolower('A'), 'a');
  EXPECT_EQ(nk_tolower('Z'), 'z');
  EXPECT_EQ(nk_tolower('a'), 'a');
  EXPECT_EQ(nk_tolower('0'), '0');
}

TEST(NkCtypeTest, ToUpper) {
  EXPECT_EQ(nk_toupper('a'), 'A');
  EXPECT_EQ(nk_toupper('z'), 'Z');
  EXPECT_EQ(nk_toupper('A'), 'A');
  EXPECT_EQ(nk_toupper('0'), '0');
}

// 边界条件测试
TEST(NkCtypeTest, BoundaryValues) {
  // ASCII 边界值
  EXPECT_FALSE(nk_isalnum(-1));
  EXPECT_FALSE(nk_isalnum(128));
  EXPECT_FALSE(nk_isalpha(127));
}

TEST(NkCtypeTest, ToLowerBoundary) {
  // 边界值
  EXPECT_EQ(nk_tolower('A' - 1), 'A' - 1);  // '@'
  EXPECT_EQ(nk_tolower('Z' + 1), 'Z' + 1);  // '['
}

TEST(NkCtypeTest, ToUpperBoundary) {
  // 边界值
  EXPECT_EQ(nk_toupper('a' - 1), 'a' - 1);  // '`'
  EXPECT_EQ(nk_toupper('z' + 1), 'z' + 1);  // '{'
}

TEST(NkCtypeTest, AllDigits) {
  for (char c = '0'; c <= '9'; ++c) {
    EXPECT_TRUE(nk_isdigit(c));
    EXPECT_TRUE(nk_isalnum(c));
    EXPECT_TRUE(nk_isxdigit(c));
  }
}

TEST(NkCtypeTest, AllLetters) {
  for (char c = 'a'; c <= 'z'; ++c) {
    EXPECT_TRUE(nk_isalpha(c));
    EXPECT_TRUE(nk_isalnum(c));
    EXPECT_TRUE(nk_islower(c));
    EXPECT_FALSE(nk_isupper(c));
  }

  for (char c = 'A'; c <= 'Z'; ++c) {
    EXPECT_TRUE(nk_isalpha(c));
    EXPECT_TRUE(nk_isalnum(c));
    EXPECT_TRUE(nk_isupper(c));
    EXPECT_FALSE(nk_islower(c));
  }
}

TEST(NkCtypeTest, AllPunctuation) {
  // 测试常见标点符号
  const char punct[] = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
  for (size_t i = 0; i < strlen(punct); ++i) {
    EXPECT_TRUE(nk_ispunct(punct[i]));
    EXPECT_FALSE(nk_isalnum(punct[i]));
  }
}

TEST(NkCtypeTest, AllWhitespace) {
  EXPECT_TRUE(nk_isspace(' '));   // space
  EXPECT_TRUE(nk_isspace('\t'));  // tab
  EXPECT_TRUE(nk_isspace('\n'));  // newline
  EXPECT_TRUE(nk_isspace('\r'));  // carriage return
  EXPECT_TRUE(nk_isspace('\f'));  // form feed
  EXPECT_TRUE(nk_isspace('\v'));  // vertical tab
}

TEST(NkCtypeTest, GraphPrintable) {
  // isgraph: 可打印但不包括空格
  // isprint: 可打印包括空格
  EXPECT_FALSE(nk_isgraph(' '));
  EXPECT_TRUE(nk_isprint(' '));

  EXPECT_TRUE(nk_isgraph('A'));
  EXPECT_TRUE(nk_isprint('A'));
}

TEST(NkCtypeTest, ControlCharacters) {
  // 测试控制字符 (0-31 和 127)
  for (int c = 0; c < 32; ++c) {
    EXPECT_TRUE(nk_iscntrl(c));
    EXPECT_FALSE(nk_isprint(c));
  }
  EXPECT_TRUE(nk_iscntrl(127));
}

TEST(NkCtypeTest, CaseConversion) {
  // 测试整个字母表的大小写转换
  for (char c = 'A'; c <= 'Z'; ++c) {
    EXPECT_EQ(nk_tolower(c), c + ('a' - 'A'));
  }

  for (char c = 'a'; c <= 'z'; ++c) {
    EXPECT_EQ(nk_toupper(c), c - ('a' - 'A'));
  }
}

TEST(NkCtypeTest, NonAscii) {
  // 测试非 ASCII 字符（假设函数处理扩展 ASCII）
  // 这些应该返回 false 对于大多数函数
  EXPECT_FALSE(nk_isalpha(128));
  EXPECT_FALSE(nk_isdigit(200));
  EXPECT_FALSE(nk_isalnum(255));
}
