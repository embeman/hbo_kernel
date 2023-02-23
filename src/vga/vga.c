#include "vga/vga.h"
#include "utils/string/string.h"
#include <stddef.h>
/*
    VGA colour defination
*/
#define VGA_BLACK           0
#define VGA_BLUE            1
#define VGA_GREEN           2
#define VGA_CYAN            3
#define VGA_RED             4
#define VGA_PURPLE          5
#define VGA_BRWON           6
#define VGA_GRAY            7
#define VGA_DARKGRAY        8
#define VGA_LIGHT_BLUE      9
#define VGA_LIGHT_GREEN    10
#define VGA_LIGHT_CYAN     11
#define VGA_LIGHT_RED      12
#define VGA_LIGHT_PURPLE   13
#define VGA_YELLOW         14
#define VGA_WHITE          15


static uint16_t* video_mem = 0;

uint8_t terminal_row =0;
uint8_t terminal_col =0;

static uint16_t terminal_make_char(char c, char colour){
    return (colour << 8 | c);
}

static void terminal_put_char(int x , int y , char c , char colour){
    video_mem[ (y * VGA_WIDTH) + x ] = terminal_make_char(c,colour);
}


static void terminal_init(){
    // base address for vga-textmode
    video_mem = (uint16_t*)0xB8000;
    // clearing out the screen
    for(int y=0 ; y<VGA_HEIGHT ; y++)
    {
        for(int x=0 ; x < VGA_WIDTH ; x++){
            // Clearing out the screen
            terminal_put_char(x,y,' ',15);
        }
    }
}

static void terminal_write_char(char c, char colour){

    if(c == '\n'){
        terminal_row += 1;
        terminal_col =0;
        return;
    }

    terminal_put_char(terminal_col , terminal_row , c , colour);
    terminal_col += 1;

    if( terminal_col >= VGA_WIDTH ){
        terminal_col =0;
        terminal_row++;
    }

}

static void terminal_write_string(const char* str , int colour){
    size_t len = strlen(str);

    for(int i=0 ; i < len ; i++)
        terminal_write_char(str[i] , colour);

}

void print(const char* str){
    // TODO : using colors to print to screen
    terminal_write_string(str , VGA_WHITE);
}

void println(const char* str){
    // TODO : using colors to print to screen
    terminal_write_string(str , VGA_WHITE);
    terminal_write_char('\n' , VGA_WHITE);
}

void print_log(const char* str){
    terminal_write_string(str , VGA_GREEN);
}

void print_warning(const char* str){
    terminal_write_string(str , VGA_YELLOW);
}

void print_error(const char* str){
    terminal_write_string(str , VGA_RED);
}

void printc(const char _c){
    terminal_write_char(_c,VGA_WHITE);
}

void print_num(uint32_t num){
    // TODO : print number to screen
    char str[10];
    int i , rem , len=0;
    uint32_t n = num;
    // find len of the number
    while (n !=0){
        len++;
        n /= 10;
    }

    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1) ] = rem + '0';
    }
    str[len] = '\0';
    print(str);

}


void vga_init(){
    terminal_init();
}
