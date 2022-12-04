#include "disk/disk.h"
#include "io/io.h"
#include "memory/memory.h"
#include "status.h"

int disk_read_sector(int lba , int total , void* buf){

    outb(0x1f6 , (lba >> 24) | 0xE0);
    outb(0x1F2 , total);
    outb(0x1F3 , (uint8_t) (lba & 0xff) );
    outb(0x1F4 , (uint8_t) (lba >> 8) );
    outb(0x1F5 , (uint8_t) (lba >> 16) );
    outb(0x1F7 , 0x20); 


    uint16_t* ptr = (uint16_t*) buf;
    for (int i = 0; i < total; i++)
    {
        uint8_t c = insb(0x1F7) ;
        
        while (! (c & 0x08))
        {
            c = insb(0x1F7);
        }

        // copy from hard disk to memory
        for (int i = 0; i < 256; i++)
        {
            *ptr = insw(0x1F0);
            ptr++;
        }
    }
    return 0;
}

static struct disk disk;

void disk_search_and_init(){
    memset(&disk , 0 , sizeof(disk));
    disk.type = KERNEL_DISK_TYPE_REAL;
    disk.sector_size = KERNEL_DISK_SECTOR_SIZE;   
}

struct disk* disk_get(int index){
    if (index !=0)
    {
        return (struct disk*)-EIO;
    }
    return &disk;
}

int disk_read_block(struct disk* idisk ,int lba , int total , void* buf){
    if (&disk != idisk)
    {
        return -EINVARG;
    }
    

    return disk_read_sector(lba,total,buf);
}


