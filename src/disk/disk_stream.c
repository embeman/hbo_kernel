#include "disk/disk_stream.h"
#include "config.h"
#include "memory/heap/kheap.h"

struct disk_stream* diskstream_new(int disk_id){
    struct disk* disk = disk_get(disk_id);
    if (!disk)
    {
        return 0;
    }
    struct disk_stream* streamer = kzalloc(sizeof(struct disk_stream));

    streamer->pos =0;
    streamer->disk = disk;

    return streamer;
}

int diskstreamer_seek(struct disk_stream* streamer , int pos)
{
    streamer->pos = pos;
    return 1;
}

// total = total bytes to read
/**
 * Reading <total> number of bytes from disk streamer <streamer> to <out> 
*/
int diskstreamer_read(struct disk_stream* streamer , int total , void* out){

    int sector = streamer->pos / KERNEL_DISK_SECTOR_SIZE;
    int offset = streamer->pos % KERNEL_DISK_SECTOR_SIZE;
    char buf[KERNEL_DISK_SECTOR_SIZE];

    int res = disk_read_block(streamer->disk , sector , 1 , buf);

    if (res < 0)
    {
        goto out;
    }

    int total_to_read = total > KERNEL_DISK_SECTOR_SIZE ? KERNEL_DISK_SECTOR_SIZE : total;
    for (int i = 0; i < total_to_read; i++)
    {
        *(char*)out++ = buf[offset + i];
    }
    // changing where the streamer points to 
    streamer->pos += total_to_read;
    if (total > KERNEL_DISK_SECTOR_SIZE)
    {
        res = diskstreamer_read(streamer , total - KERNEL_DISK_SECTOR_SIZE , out);
    }

out:
    return res;
}

void diskstreamr_free(struct disk_stream* streamer){
    kfree(streamer);
}

