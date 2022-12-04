#include "memory/memory.h"



int memcmp(void* p1 , void* p2 , size_t size){
    char* c1 = p1;
    char* c2 = p2;

    while (size-- > 0)
    {
        if (*c1++ != *c2++)
        {
            return c1[-1] > c2[-1] ? -1 : 1 ;
        }
    }
    
    return 0;
}

void* memset(void* ptr, int c , int size){

    char* c_ptr = (char*) ptr;
    for(int i=0 ; i < size ; i ++)
    {
        c_ptr[i] = (char) c; 
    }
    return ptr;
}
