#ifndef IDT_H
#define IDT_H

#include <stdint.h>

void idt_init();
extern void enable_interrupt();
extern void disable_intterupt();

/*

    interrupt descriptor table entry 
    the size of each enrty is 8-byte lone ( 64-bits )
    each entry represent interrupt ( gate )  
*/
struct idt_desc{
    uint16_t offset_1;      // first offset 0-15
    uint16_t selector;      // code segment
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_2;
}__attribute__((packed));

/*
    interrupt descriptor table IDT 
    this structuer represent the value stored in idtr register 
    it is 48-bit long 
    the fisrt 16-bits contains the size of our descriptor table 
    the last 32-bits contains the base address of our descriptor table
*/
struct idtr_desc{
    uint16_t limit;     // size of the descriptor table - 1
    uint32_t base;      
}__attribute__((packed));

#endif