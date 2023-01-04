#include "fs/fs.h"
#include "fs/fat16/fat16.h"

// disk inclusion
#include "disk/disk.h"
#include "disk/disk_stream.h"

int fs_init(){
    // searching the inilizing the disks
    disk_search_and_init();

    return fat16_init();
}

void open(){

}

void read(){

}




