/**
 * @copyright Copyright The NaiLong-Kernel Contributors
 */

#include <gtest/gtest.h>

// Rename functions to avoid conflict with standard library
#define atof nk_atof
#define atoi nk_atoi
#define atol nk_atol
#define atoll nk_atoll
#define strtod nk_strtod
#define strtof nk_strtof
#define strtold nk_strtold
#define strtol nk_strtol
#define strtoll nk_strtoll
#define strtoul nk_strtoul
#define strtoull nk_strtoull

#include "../../src/libc/nk_stdlib.c"

TEST(NkLibcTest, Atoi) {
  EXPECT_EQ(nk_atoi("123"), 123);
  EXPECT_EQ(nk_atoi("-123"), -123);
  EXPECT_EQ(nk_atoi("+123"), 123);
  EXPECT_EQ(nk_atoi("0"), 0);
  EXPECT_EQ(nk_atoi("   456"), 456);  // Leading spaces
  EXPECT_EQ(nk_atoi("789abc"), 789);  // Stop at non-digit
}

TEST(NkLibcTest, Atol) {
  EXPECT_EQ(nk_atol("123456789"), 123456789L);
  EXPECT_EQ(nk_atol("-123456789"), -123456789L);
}

TEST(NkLibcTest, Atoll) {
  EXPECT_EQ(nk_atoll("123456789012345"), 123456789012345LL);
  EXPECT_EQ(nk_atoll("-123456789012345"), -123456789012345LL);
}

TEST(NkLibcTest, Atof) {
  EXPECT_DOUBLE_EQ(nk_atof("3.14"), 3.14);
  EXPECT_DOUBLE_EQ(nk_atof("-2.5"), -2.5);
  EXPECT_DOUBLE_EQ(nk_atof("0.0"), 0.0);
  EXPECT_DOUBLE_EQ(nk_atof("123.456"), 123.456);
}

TEST(NkLibcTest, Strtol) {
  char *end;
  EXPECT_EQ(nk_strtol("123", &end, 10), 123L);
  EXPECT_EQ(*end, '\0');

  EXPECT_EQ(nk_strtol("-123", &end, 10), -123L);
  EXPECT_EQ(nk_strtol("   100", &end, 10), 100L);
  EXPECT_EQ(*end, '\0');

  // Base 16
  EXPECT_EQ(nk_strtol("0xABC", &end, 16), 2748L);
  EXPECT_EQ(*end, '\0');
  EXPECT_EQ(nk_strtol("-0xabc", &end, 16), -2748L);

  // Auto detect base
  EXPECT_EQ(nk_strtol("0x10", &end, 0), 16L);
  EXPECT_EQ(nk_strtol("010", &end, 0), 8L);
  EXPECT_EQ(nk_strtol("10", &end, 0), 10L);

  // Stop at invalid
  EXPECT_EQ(nk_strtol("123xyz", &end, 10), 123L);
  EXPECT_EQ(*end, 'x');
}

TEST(NkLibcTest, Strtoul) {
  char *end;
  EXPECT_EQ(nk_strtoul("123", &end, 10), 123UL);
  EXPECT_EQ(nk_strtoul("0xFF", &end, 16), 255UL);
  EXPECT_EQ(nk_strtoul("11", &end, 2), 3UL);
}

TEST(NkLibcTest, Strtoll) {
  char *end;
  // min for 64bit signed
  // -9223372036854775808
  EXPECT_EQ(nk_strtoll("-9223372036854775808", &end, 10),
            -9223372036854775807LL - 1);
}

TEST(NkLibcTest, Strtoull) {
  char *end;
  // max for 64bit unsigned
  // 18446744073709551615
  EXPECT_EQ(nk_strtoull("18446744073709551615", &end, 10),
            18446744073709551615ULL);
}

TEST(NkLibcTest, Strtod) {
  char *end;
  EXPECT_DOUBLE_EQ(nk_strtod("3.14159", &end), 3.14159);
  EXPECT_EQ(*end, '\0');
  EXPECT_DOUBLE_EQ(nk_strtod("  -123.456abc", &end), -123.456);
  EXPECT_EQ(*end, 'a');
}

TEST(NkLibcTest, Strtof) {
  char *end;
  EXPECT_FLOAT_EQ(nk_strtof("3.14", &end), 3.14f);
}

// 边界条件测试
TEST(NkLibcTest, AtoiEdgeCases) {
  // 空格和前导零
  EXPECT_EQ(nk_atoi("   000123"), 123);

  // 最大/最小值（在 int 范围内）
  EXPECT_EQ(nk_atoi("2147483647"), 2147483647);
  EXPECT_EQ(nk_atoi("-2147483648"), -2147483648);

  // 无效输入
  EXPECT_EQ(nk_atoi("abc"), 0);
  EXPECT_EQ(nk_atoi(""), 0);
}

TEST(NkLibcTest, AtolEdgeCases) {
  // 长整型范围
  EXPECT_EQ(nk_atol("9223372036854775807"), 9223372036854775807L);
  EXPECT_EQ(nk_atol("-9223372036854775808"), -9223372036854775807L - 1);
}

TEST(NkLibcTest, AtofEdgeCases) {
  // 科学计数法（如果支持）
  EXPECT_DOUBLE_EQ(nk_atof("1e2"), 100.0);
  EXPECT_DOUBLE_EQ(nk_atof("1.5e-2"), 0.015);

  // 零
  EXPECT_DOUBLE_EQ(nk_atof("0.0"), 0.0);
  EXPECT_DOUBLE_EQ(nk_atof("-0.0"), -0.0);

  // 小数点
  EXPECT_DOUBLE_EQ(nk_atof(".5"), 0.5);
  EXPECT_DOUBLE_EQ(nk_atof("5."), 5.0);
}

TEST(NkLibcTest, StrtolEdgeCases) {
  char *end;

  // 空字符串
  EXPECT_EQ(nk_strtol("", &end, 10), 0L);

  // 只有符号
  EXPECT_EQ(nk_strtol("+", &end, 10), 0L);
  EXPECT_EQ(nk_strtol("-", &end, 10), 0L);

  // 二进制数字（注意：标准 strtol 不识别 "0b" 前缀）
  EXPECT_EQ(nk_strtol("1010", &end, 2), 10L);
  EXPECT_EQ(nk_strtol("11111111", &end, 2), 255L);

  // 负的十六进制
  EXPECT_EQ(nk_strtol("-0xFF", &end, 16), -255L);
}

TEST(NkLibcTest, StrtoulEdgeCases) {
  char *end;

  // 最大无符号值
  EXPECT_EQ(nk_strtoul("4294967295", &end, 10), 4294967295UL);

  // 八进制
  EXPECT_EQ(nk_strtoul("0777", &end, 8), 511UL);

  // 二进制
  EXPECT_EQ(nk_strtoul("1111", &end, 2), 15UL);
}

TEST(NkLibcTest, StrtollEdgeCases) {
  char *end;

  // 零
  EXPECT_EQ(nk_strtoll("0", &end, 10), 0LL);

  // 负的最大值
  EXPECT_EQ(nk_strtoll("-9223372036854775807", &end, 10),
            -9223372036854775807LL);
}

TEST(NkLibcTest, StrtoullEdgeCases) {
  char *end;

  // 零
  EXPECT_EQ(nk_strtoull("0", &end, 10), 0ULL);

  // 十六进制最大值
  EXPECT_EQ(nk_strtoull("FFFFFFFFFFFFFFFF", &end, 16), 18446744073709551615ULL);
}

TEST(NkLibcTest, StrtodEdgeCases) {
  char *end;

  // 无穷大（如果支持）
  // EXPECT_TRUE(std::isinf(nk_strtod("inf", &end)));

  // 非常小的数
  EXPECT_DOUBLE_EQ(nk_strtod("0.000001", &end), 0.000001);

  // 非常大的数
  EXPECT_DOUBLE_EQ(nk_strtod("123456789.0", &end), 123456789.0);
}

TEST(NkLibcTest, StrtofEdgeCases) {
  char *end;

  // 零
  EXPECT_FLOAT_EQ(nk_strtof("0.0", &end), 0.0f);

  // 负数
  EXPECT_FLOAT_EQ(nk_strtof("-1.5", &end), -1.5f);
}

TEST(NkLibcTest, BaseDetection) {
  char *end;

  // Base 0 应该自动检测
  EXPECT_EQ(nk_strtol("0x10", &end, 0), 16L);
  EXPECT_EQ(nk_strtol("010", &end, 0), 8L);
  EXPECT_EQ(nk_strtol("10", &end, 0), 10L);
}

TEST(NkLibcTest, WhitespaceHandling) {
  // 测试各种空白字符
  EXPECT_EQ(nk_atoi("  \t\n\r123"), 123);
  EXPECT_EQ(nk_atoi("\t\t\t456"), 456);
}

TEST(NkLibcTest, SignHandling) {
  // 测试符号处理
  EXPECT_EQ(nk_atoi("+123"), 123);
  EXPECT_EQ(nk_atoi("-123"), -123);
  EXPECT_EQ(nk_atoi("++123"), 0);  // 双符号应该无效
  EXPECT_EQ(nk_atoi("--123"), 0);
}

TEST(NkLibcTest, PartialConversion) {
  char *end;

  // 部分转换
  EXPECT_EQ(nk_strtol("123abc", &end, 10), 123L);
  EXPECT_STREQ(end, "abc");

  EXPECT_EQ(nk_strtol("0xFFGG", &end, 16), 255L);
  EXPECT_STREQ(end, "GG");
}
