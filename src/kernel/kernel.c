#include "kernel/kernel.h"
#include "memory/heap/kheap.h"
#include "idt/idt.h"
#include "vga/vga.h"
#include "io/io.h"
#include "memory/paging/paging.h"

#include "fs/fs.h"

#include <stdint.h>
#include <stddef.h>

#include <utils/printf.h>

static struct paging_4gb_chunk* kernel_chunk ;

void panic(const char* msg){
    print_error(msg);
    while (1){}
}

uint16_t pci_data_port = 0xcfc;
uint16_t pci_command_port = 0xcf8;

uint32_t pci_read(uint8_t bus , uint8_t device ,uint8_t func ,uint8_t offset){
    uint32_t id = 
            ( bus << 16 ) |                 // bus at bit 16-23
            ((device & 0x1f) << 11 ) |      // device -slot- bit 11-15
            ( (func & 0x07) << 8 ) |        // func bit 8-10
            (offset & 0xfc) |               // offset bit 0-7
            ( 0x80000000 );                 // set the enable bit 31
    
    outl(pci_command_port , id);

    uint32_t res = insl(pci_data_port);
    return res >> ( 8 * (offset % 4));
}

void pci_write(uint8_t bus , uint8_t device ,uint8_t func ,uint8_t offset , uint32_t value){
    uint32_t id = 
            ( bus << 16 ) |                 // bus at bit 16-23
            ((device & 0x1f) << 11 ) |      // device -slot- bit 11-15
            ( (func & 0x07) << 8 ) |        // func bit 8-10
            (offset & 0xfc) |               // offset bit 0-7
            ( 0x80000000 );                 // set the enable bit 31

    outl(pci_command_port , id);
    outl(pci_data_port , value);
}

uint8_t pci_device_has_function(uint8_t bus , uint8_t device){
    uint32_t res = pci_read(bus,device ,0 ,0x0E);
    return (res & (1 << 7));
}

uint8_t pci_check_device_availabel(uint8_t bus , uint8_t device){
    uint32_t res = pci_read(bus,device ,0 ,0);
    if (res == 0xffffffff)
    {
        return 0;
    }
    return 1;
}

void kernel_main()
{
    vga_init();
    kprintf("Kernel Booted..\n");
    // setting up kernel heap
    kheap_init();
    // setup paging 
    kernel_chunk = paging_new_4gb(PAGING_IS_PRESENT | PAGING_WRITE_FROM_ALL | PAGING_IS_PRESENT);
    // switch to kernel paging chunk
    paging_switch(kernel_chunk->directory_entry);
    // enable paging
    enable_paging();
    // load interrupt descriptor table and enable interrupts
    idt_init();
    // Reading and Inilizing Filesystems 
    (void)fs_init();
    kprintf("Kernel Initliazed..\n");

    kprintf("Hello World \n" , 0);
    kprintf("Hello World %d : %d \n" ,100 , 200 , 400);



    while (1);    
}
