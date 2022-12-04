#ifndef HEAP_H
#define HEAP_H

#include "config.h"
#include <stdint.h>
#include <stddef.h>


#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 0b00000001
#define HEAP_BLOCK_TABLE_ENTRY_FREE  0b00000000

#define HEAP_BLOCK_HAS_NEXT 0b10000000
#define HEAP_BLOCK_IS_FIRST 0b01000000


typedef uint8_t HEAP_BLOCK_TABLE_ENTRY; 

// heap_table contains all entries in our heap
struct heap_table{
    HEAP_BLOCK_TABLE_ENTRY* entries;
    size_t total_entries;
};
// heap contains our heap_table and the start address of the heap
struct heap{
    struct heap_table* table;
    void* heap_start_addr;
};


int heap_create(struct heap* heap , void* ptr , void* end , struct heap_table* table);
void* heap_malloc(struct heap* heap, size_t size);
void heap_free(struct heap* heap , void* ptr);

#endif