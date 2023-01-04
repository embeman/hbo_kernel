#ifndef _FAT16_PRIVATE_H_
#define _FAT16_PRIVATE_H_


// 36 byte
struct fat_header_primary{
    uint8_t short_jmp_ins[3];
    uint8_t oem_identifier[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_copies;
    uint16_t root_dir_entries;
    uint16_t number_of_sectors;
    uint8_t media_type;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t number_of_heads;
    uint32_t hidden_setors;
    uint32_t sectors_big;
} __attribute__((packed));

// 26 byte
struct fat_header_extended
{
    uint8_t drive_number;
    uint8_t win_nt_bit;
    uint8_t signature;
    uint32_t volume_id;
    uint8_t volume_id_string[11];
    uint8_t system_id_string[8];
} __attribute__((packed));


struct fat_header
{
    struct fat_header_primary primary_header;
    struct fat_header_extended extended_header;
};


// 32 byte
struct fat_directory_entry
{
    uint8_t filename[8];                    // entry name
    uint8_t ext[3];                         // entry extension
    uint8_t attribute;                      // FAT file attribute
    uint8_t reserved;                       // reserved
    uint8_t creation_time_tenths_of_a_sec;  // creation time in ms
    uint16_t creation_time;                 // creation time in (Hour: 5 bits , Minutes: 6 bits, Seconds : 5 bits )
    uint16_t creation_date;                 // creation date in (Year : 7 bits , Month : 4 bits , Day : 5 bits)
    uint16_t last_access;                   // Last accessed date. Same format as the creation date.
    uint16_t high_16_bits_first_cluster;    // The high 16 bits of this entry's first cluster number. For FAT 12 and FAT 16 this is always zero.
    uint16_t last_mod_time;                 // Last modification time. Same format as the creation time.
    uint16_t last_mod_date;                 // Last modification date. Same format as the creation date.
    uint16_t low_16_bits_first_cluster;     // The low 16 bits of this entry's first cluster number. Use this number to find the first cluster for this entry.
    uint32_t filesize;                      // filesize in bytes
} __attribute__((packed));

struct fat_private{
    // this structer contains all private data of our driver

    // information about file-system
    // reserved sector data
    uint32_t reserved_region_start;
    uint32_t reserved_region_end;
    // fat table data
    uint32_t fat_table_start;
    uint32_t fat_table_end;
    // root_dir region start
    uint32_t root_dir_start;
    uint32_t root_dir_end;

    uint16_t first_root_dir_sector;
    uint16_t first_cluster_sector;

    uint16_t* fat_table;
};


#define FAT_FILE_ATTR_READ_ONLY 0x01
#define FAT_FILE_ATTR_HIDDEN 0x02
#define FAT_FILE_ATTR_SYSTEM 0x04
#define FAT_FILE_ATTR_VOLUME_LABEL 0x08
#define FAT_FILE_ATTR_SUBDIRECTORY 0x10
#define FAT_FILE_ATTR_ARCHIVED 0x20
#define FAT_FILE_ATTR_DEVICE 0x40
#define FAT_FILE_ATTR_RESERVED 0x80


#endif
