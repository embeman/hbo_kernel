#ifndef CONFIG_H
#define CONFIG_H


#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10

// kernel disk sector size
#define KERNEL_DISK_SECTOR_SIZE 512

// kernel disk type
#define KERNEL_DISK_TYPE_REAL 1



#define KERNEL_MAX_PATH 108

#define TOTAL_NO_OF_INTERRUPT 256



/*
    KERNEL HEAP CONFIGURATION
*/

// 100M
#define KERNEL_HEAP_SIZE_BYTES 104857600
// block size
#define KERNEL_HEAP_BLOCK_SIZE 4096
// heap address ( our data pool )
#define KERNEL_HEAP_ADDRESS 0x01000000
// heap table address 
#define KERNEL_HEAP_TABLE_ADDRESS 0x00007E00

#endif
