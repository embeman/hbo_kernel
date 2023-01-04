#ifndef VGA_H
#define VGA_H

#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

void print(const char* str);
void println(const char* str);
void print_log(const char* str);
void print_warning(const char* str);
void print_error(const char* str);

void printc(const char _c);
void vga_init();
#endif