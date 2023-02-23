#include <stdarg.h>

#include "vga/vga.h"
#include <utils/string/string.h>

/**
 * 
 * Simple printf like cabable of handling <%d> format-specifier
 * 
**/

int kprintf(const char* str , ...){
    va_list args_lis;
    va_start(args_lis , str);
    int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (str[i] == '%')
        {
            i++;
            switch (str[i]){
                case 'd':
                    print_num(va_arg(args_lis , int));
                    break;
                default:
                    break;
            }
        }else{
            printc(str[i]);
        }     
    }
    va_end(args_lis);
    return 0;
}