#ifndef PPARSER_H_
#define PPARSER_H_



struct path_part{
    const char* str;
    struct path_part* next;
};

struct path_root{
    int drive_no;
    struct path_part* first;
};

struct path_root* pathparse_parse(const char* path , const char* current_directory_path);


#endif
