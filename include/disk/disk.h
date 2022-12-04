#ifndef DISK_H_
#define DISK_H_


typedef unsigned int KERNEL_DISK_TYPE;

struct disk
{
    KERNEL_DISK_TYPE type;
    int sector_size;
};



int disk_read_sector(int lba , int total , void* buf);
void disk_search_and_init();
struct disk* disk_get(int index);
int disk_read_block(struct disk* idisk ,int lba , int total , void* buf);

#endif
