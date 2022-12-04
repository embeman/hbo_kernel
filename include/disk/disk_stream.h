#ifndef DISK_STREAM_H_
#define DISK_STREAM_H_

#include "disk/disk.h"

struct disk_stream
{
    int pos;
    struct disk* disk; 
};

struct disk_stream* diskstream_new(int disk_id);
int diskstreamer_seek(struct disk_stream* streamer , int pos);
int diskstreamer_read(struct disk_stream* streamer , int total , void* out);
void diskstreamer_free(struct disk_stream* streamer);


#endif