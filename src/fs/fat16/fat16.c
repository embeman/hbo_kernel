#include "fs/fat16/fat16.h"
#include "fs/fat16/fat16_private.h"
#include "disk/disk_stream.h"
#include "vga/vga.h"
#include "status.h"
#include "memory/heap/kheap.h"


#define SECTOR_TO_BYTE_OFFSET(X) ( (X) * 512 )


static struct fat_header fat_h;
static struct fat_private fat_p;
struct disk_stream* streamer_disk_0;

static void init_private(void){

    // reserved sector data
    fat_p.reserved_region_start =0;
    fat_p.reserved_region_end = fat_h.primary_header.reserved_sectors;
    // fat table data
    fat_p.fat_table_start = fat_p.reserved_region_end;
    fat_p.fat_table_end = fat_p.fat_table_start + (fat_h.primary_header.fat_copies * fat_h.primary_header.sectors_per_fat);
    // root_dir region start
    fat_p.root_dir_start = fat_p.fat_table_end;
    fat_p.root_dir_end = fat_p.root_dir_start 
        + ( ( (fat_h.primary_header.root_dir_entries * 32) + (fat_h.primary_header.bytes_per_sector - 1) )
                        / fat_h.primary_header.bytes_per_sector );


    fat_p.first_root_dir_sector = fat_p.fat_table_end;
    fat_p.first_cluster_sector = fat_p.root_dir_end;
    
}



int fat16_init(){
    int res = 0;

    // creating a new disk stream for fat16 
    // fat16 is always at disk 0 ( the only disk we implemented so far )
    streamer_disk_0 = diskstream_new(0);

    // Reading Fat16 information from the disk 
    // reading Fat Header from sector 0
    diskstreamer_read(streamer_disk_0 , sizeof(struct fat_header_primary) , &(fat_h.primary_header));
    // Reading Fat Extended Header
    diskstreamer_read(streamer_disk_0 , sizeof(struct fat_header_extended) , &(fat_h.extended_header));

    init_private();

    // reading root directory entry
    struct fat_directory_entry* root_entries = (struct fat_directory_entry*)kzalloc(sizeof(struct fat_directory_entry) * fat_h.primary_header.root_dir_entries);
    diskstreamer_seek(streamer_disk_0 , SECTOR_TO_BYTE_OFFSET(fat_p.first_root_dir_sector));
    diskstreamer_read(streamer_disk_0 , ( sizeof(struct fat_directory_entry) * fat_h.primary_header.root_dir_entries ) , root_entries);


out:
    return res;
}