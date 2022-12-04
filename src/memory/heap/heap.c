#include "memory/heap/heap.h"
#include "memory/memory.h"
#include "status.h"

// calculate total entries and confirms the our heap_table structure is valid
static uint8_t heap_validate_table(void* ptr , void* end , struct heap_table* table){
    int res =0 ;
    size_t table_size = (size_t) (end - ptr);
    size_t total_entries = table_size / KERNEL_HEAP_BLOCK_SIZE;

    if (total_entries != table->total_entries)
    {
        res = -EINVARG;
        goto out;
    }
out:
    return res;
}

// make sure the heap is aligned to configured block size
static uint32_t heap_validate_alignment(void *ptr){
    return  ((uint32_t)ptr % KERNEL_HEAP_BLOCK_SIZE) == 0 ;
}

/*
    check the validation of our heap structure
*/
int heap_create(struct heap* heap , void* ptr , void* end , struct heap_table* table){
    int res =0;
    // check alignment of address to our block size 
    if (  !heap_validate_alignment(ptr) || !heap_validate_alignment(end)  ){
        res = -EINVARG;
        goto out;
    }
    memset(heap , 0 , sizeof(struct heap));
    heap->heap_start_addr = ptr;
    heap->table = table;

    res = heap_validate_table(ptr, end , table);
    if (res < 0)
    {
        res = -EINVARG;
        goto out;       
    }

    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->total_entries;
    memset(table->entries , HEAP_BLOCK_TABLE_ENTRY_FREE , table_size);
out:
    return res;
}

// calculate total number of bytes to allocate
static uint32_t heap_align_value_to_upper(uint32_t val){
    if ( ( val % KERNEL_HEAP_BLOCK_SIZE ) == 0)
    {
        return val;
    }

    val = val - (val % KERNEL_HEAP_BLOCK_SIZE);
    val +=  KERNEL_HEAP_BLOCK_SIZE;

    return val;

}

static uint8_t heap_get_entry_type(HEAP_BLOCK_TABLE_ENTRY entry){
    return entry & 0x0f;
}


/*
    find start block of contiguos blocks marked with ( HEAP_BLOCK_TABLE_ENTRY_FREE ) 
*/
static int heap_get_start_block(struct heap* heap , size_t total_blocks){

    struct heap_table* table = heap->table;
    int bc = 0;     // current block 
    int bs = -1;    // start block 

    for (size_t i = 0; i < table->total_entries; i++)
    {
        if (heap_get_entry_type(table->entries[i]) != HEAP_BLOCK_TABLE_ENTRY_FREE )
        {
            // reset the state
            bc =0 ;
            bs = -1;
            continue;
        }
        // is this is the first free block
        if (bs == -1)
        {
            bs = i;
        }
        bc++;

        if (bc == total_blocks)
        {
            break;
        }        
    }
    // check if we out of the loop with no starting block 
    if (bs == -1)
    {
        return -ENOMEM;
    }
    return bs;
}

// convert block index in heap to addres to return to the user 
static void* heap_block_to_address(struct heap* heap , uint32_t start_block){
    return heap->heap_start_addr + (start_block * KERNEL_HEAP_BLOCK_SIZE);
}

// mark blocks from start_block to ( start_block + total_block -1 ) as taken
static void heap_mark_block_taken(struct heap* heap , int start_block , size_t total_blocks){
    int end_block = (start_block + total_blocks) - 1 ;

    HEAP_BLOCK_TABLE_ENTRY entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;

    if (total_blocks > 1)
    {
        entry |= HEAP_BLOCK_HAS_NEXT;
    }
    for (size_t i = start_block; i <= end_block; i++)
    {
        heap->table->entries[i] = entry;
        entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
        if (i != end_block - 1)
        {
            entry |= HEAP_BLOCK_HAS_NEXT;
        }
    }
}
/*
    allocate blocks for our heap
*/
static void* heap_malloc_blocks(struct heap* heap , size_t total_blocks){
    void* address =0;

    int start_block = heap_get_start_block(heap , total_blocks);

    if (start_block < 0 )
    {
        // errro can't find total block contiguos
        goto out;
    }
    
    address = heap_block_to_address(heap,start_block);

    // Mark the block as taken
    heap_mark_block_taken(heap , start_block , total_blocks);

out:
    return address;
}

static uint32_t heap_address_to_block(struct heap* heap , void* ptr){
    return (uint32_t) (ptr - heap->heap_start_addr) / KERNEL_HEAP_BLOCK_SIZE ;       
}

static void heap_mark_block_free(struct heap* heap , uint32_t starting_block){
    struct heap_table* table = heap->table;
    for (int i = starting_block; i < (int)table->total_entries; i++)
    {
       HEAP_BLOCK_TABLE_ENTRY entry = table->entries[i];
       table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;
       if (!(entry & HEAP_BLOCK_HAS_NEXT))
       {
            break;
       }       
    }
}



void* heap_malloc(struct heap* heap , size_t size){

    size_t aligned_size = heap_align_value_to_upper(size);
    uint32_t total_blocks = aligned_size / KERNEL_HEAP_BLOCK_SIZE;
    return heap_malloc_blocks(heap,total_blocks);
}



void heap_free(struct heap* heap , void* ptr){
    heap_mark_block_free(heap , heap_address_to_block(heap , ptr));
}


