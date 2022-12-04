#include "fs/pparser.h"
#include "status.h"
#include "memory/memory.h"
#include "utils/string/string.h"
#include "memory/heap/kheap.h"

static int pathparser_valid_formate(const char* filename){

    int len = strnlen(filename , KERNEL_MAX_PATH);
    return ( ( len  >= 3 ) && ( is_digit(filename[0]) )&& ( memcmp((void*)&filename[1] , ":/" , 2) == 0 ) );
}

static int pathparser_get_drive_by_path(const char** filename){

    if (!pathparser_valid_formate(*filename))
    {
        return -EBADPATH;
    }
    int drive_num = char_to_num(*filename[0]);

    // add 3 byte to skip number
    *filename += 3;
    
    return drive_num;
}

static struct path_root* pathparser_create_root(int drive_num){

    struct path_root* path_r = kzalloc(sizeof(struct path_root));

    path_r->drive_no = drive_num;
    path_r->first = 0;
    return path_r;
}


static const char* pathparser_get_path_part(const char **path){
    char* result_path_part = kmalloc(KERNEL_MAX_PATH);

    int i=0;    
    while(**path != '/'  && **path != 0x00){
        result_path_part[i] = **path;
        *path += 1;
        i++;
    }

    if (**path == '/')
    {
        // skip the forward slash
        *path +=1;
    }

    if (i == 0)
    {
        kfree(result_path_part);
        result_path_part =0;
    }
    return result_path_part;
}


struct path_part* pathparser_parse_path_part(struct path_part* last ,const char** path){
    
    const char *path_part_str = pathparser_get_path_part(path);

    if (!path_part_str)
    {
        return 0;
    }
    
    struct path_part* part = kzalloc(sizeof(struct path_part));

    part->str = path_part_str;
    part->next = 0x00;

    if (last)
    {
        last->next = part;
    }
    return part;  

} 

void pathparser_free(struct path_root* root){
    struct path_part* part = root->first;

    while (part)
    {
        struct path_part* next_part = part->next;
        kfree((void*)part->str);
        kfree(part);
        part = next_part;
    }
    kfree(root);

}

struct path_root* pathparse_parse(const char* path , const char* current_directory_path){

    int res =0 ;

    const char* tmp_path = path;
    struct path_root* root =0;

    if (strlen(path) > KERNEL_MAX_PATH)
    {
        // our path is longer than our KERNEL_MAX_PATH
        goto out;
    }
    
    res = pathparser_get_drive_by_path(&tmp_path);
    if (res < 0)
    {
        goto out;
    }

    root = pathparser_create_root(res);
    
    if (!root)
    {
        goto out;
    }

    struct path_part* first = pathparser_parse_path_part(NULL , &tmp_path);
    if (!first)
    {
        goto out;
    }

    root->first = first;
    struct path_part* part = pathparser_parse_path_part(first , &tmp_path);
    while(part){
        part = pathparser_parse_path_part(part , &tmp_path);
    }
out:
    return root;
} 
