#ifndef PAGING_H_
#define PAGING_H_


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h> 
#define PAGING_CACHE_DISABLED   0b0010000
#define PAGING_WRITE_THROUGH    0b0001000
#define PAGING_WRITE_FROM_ALL   0b0000100
#define PAGING_IS_WRITEABLE     0b0000010
#define PAGING_IS_PRESENT       0b0000001


#define PAGING__TOTAL_ENTRIES_PER_TABLE 1024
#define PAGING_PAGE_SIZE 4096

struct paging_4gb_chunk
{
    // pointer to page directory
    uint32_t* directory_entry;
};
struct paging_4gb_chunk* paging_new_4gb(uint8_t flags);
void paging_switch(uint32_t* directory);
void enable_paging();
uint32_t* paging_4gb_chunk_get_directory(struct paging_4gb_chunk* chunk);
bool paging_is_aligned(void *addr);
int paging_set(uint32_t* directory , void* virt , uint32_t val);
#endif
