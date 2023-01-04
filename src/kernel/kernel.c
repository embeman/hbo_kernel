#include "kernel/kernel.h"
#include "memory/heap/kheap.h"
#include "idt/idt.h"
#include "vga/vga.h"
#include "io/io.h"
#include "memory/paging/paging.h"

#include "fs/fs.h"

#include <stdint.h>
#include <stddef.h>

static struct paging_4gb_chunk* kernel_chunk ;

void kernel_main()
{
    vga_init();
    print_log("Kernel Booted..\n");
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
    
    while (1);    
}
