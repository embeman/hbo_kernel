#include "memory/heap/kheap.h"
#include "memory/heap/heap.h"
#include "memory/memory.h"
#include "vga/vga.h"
#include "config.h"

struct heap kernel_heap;
struct heap_table kernel_heap_table;

void kheap_init(){
    // calculating how many entries in our kernel heap
    int total_table_entries = KERNEL_HEAP_SIZE_BYTES / KERNEL_HEAP_BLOCK_SIZE;
    kernel_heap_table.total_entries = total_table_entries;
    kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY*) KERNEL_HEAP_TABLE_ADDRESS;

    // calculating the end of our data pool
    void* end = (void*) (KERNEL_HEAP_ADDRESS + KERNEL_HEAP_SIZE_BYTES);

    int res = heap_create(&kernel_heap , (void*)KERNEL_HEAP_ADDRESS , end , &kernel_heap_table);
    if (res < 0)
    {
        // error 
        // faild to create heap 
        // TO DO : kernel panic
        print("Faild to create heap");
    }
}

void* kmalloc(size_t size){
    return heap_malloc(&kernel_heap , size);
}

void* kzalloc(size_t size){
    void* ptr = kmalloc(size);
    if (!ptr)
        return 0;
    
    memset(ptr,0x00 , size);
    return ptr;
}

void kfree(void* ptr){
    heap_free(&kernel_heap,ptr);
}