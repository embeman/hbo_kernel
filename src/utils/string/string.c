#include "utils/string/string.h"


size_t strlen(const char* str){
    size_t len =0;
    while(str[len]){
        len++;
    }
    return len;
}

size_t strnlen(const char* str , int max){

    int i=0;
    for (i = 0; i < max; i++)
    {
        if (str[i] == 0)
        {
            break;
        }
        
    }
    return i;
}


bool is_digit(char c){
    return ( c >= 48 ) && ( c <= 57 ); 
}

uint8_t char_to_num(char c){
    return c - 48;
}



