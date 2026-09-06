
/**
 * @file main.c
 * @brief main c
 * @author Zone.N (Zone.Niuzh@hotmail.com)
 * @version 1.0
 * @date 2024-03-05
 * @copyright MIT LICENSE
 * https://github.com/MRNIU/opensbi_interface
 * @par change log:
 * <table>
 * <tr><th>Date<th>Author<th>Description
 * <tr><td>2024-03-05<td>Zone.N (Zone.Niuzh@hotmail.com)<td>创建文件
 * </table>
 */

#include "opensbi_interface.h"

int strlen(const char *_src) {
  int idx = 0;
  while (_src[idx] != '\0') {
    idx++;
  }
  return idx;
}

int main(int, char **) {
  sbi_debug_console_write_byte('H');
  sbi_debug_console_write_byte('e');
  sbi_debug_console_write_byte('l');
  sbi_debug_console_write_byte('l');
  sbi_debug_console_write_byte('o');
  sbi_debug_console_write_byte('W');
  sbi_debug_console_write_byte('o');
  sbi_debug_console_write_byte('r');
  sbi_debug_console_write_byte('l');
  sbi_debug_console_write_byte('d');
  sbi_debug_console_write_byte('!');
  sbi_debug_console_write_byte('\n');

  char *aaa = "HelloOpenSBI!\n";

  sbi_debug_console_write(strlen(aaa), (unsigned long)aaa & 0xffffffff,
                          (unsigned long)aaa >> 32);
  return 0;
}
