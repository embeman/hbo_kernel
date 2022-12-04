#ifndef STRING_H_
#define STRING_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

size_t strlen(const char* str);
size_t strnlen(const char* str , int max);
bool is_digit(char c);
uint8_t char_to_num(char c);

#endif