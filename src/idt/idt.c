#include "idt/idt.h"
#include "io/io.h"
#include "vga/vga.h"
#include "memory/memory.h"
#include "utils/string/string.h"
#include "config.h"

// found in idt.asm
extern void idt_load(struct idtr_desc* ptr);
extern void int21h();
extern void no_interrupt();

// defined in idt.h
struct idt_desc idt_descriptor[TOTAL_NO_OF_INTERRUPT];
struct idtr_desc idtr_descriptor;

// isr routines
void no_interrupt_handler();
void int21h_handler();

void idt_set(int interrupt_no , void* address){

    struct idt_desc* desc = &idt_descriptor[interrupt_no]; 

    desc->offset_1 = (uint32_t) address & 0x0000ffff;
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->zero = 0;
    /*
        type_attr is 8-bit long contains the following :
            Gate Type (0-3 bits ):  A 4-bit value which defines the type of gate this Interrupt Descriptor represents. There are five valid type values:
                                    0b0101 or 0x5: Task Gate, note that in this case, the Offset value is unused and should be set to zero.
                                    0b0110 or 0x6: 16-bit Interrupt Gate
                                    0b0111 or 0x7: 16-bit Trap Gate
                                    0b1110 or 0xE: 32-bit Interrupt Gate
                                    0b1111 or 0xF: 32-bit Trap Gate
            Bit (4) = 0 reserved
            DPL ( 5-6 bits = 0b11 hence can be called from user program ): A 2-bit value which defines the CPU Privilege Levels which are allowed to access this interrupt via the INT instruction. Hardware interrupts ignore this mechanism.
            Note : 
                    DPL mechanism is used to create interrupts valid only to be called by the kernel
            
            
            P (7 bit): Present bit. Must be set (1) for the descriptor (aka Entry) to be valid.
    */
    desc->type_attr = 0b11101110;
    desc->offset_2 = (uint32_t) address >> 16;

}

void idt_init(){
    memset(idt_descriptor , 0 , sizeof(idt_descriptor));
    idtr_descriptor.limit = sizeof(idt_descriptor) -1 ;
    idtr_descriptor.base = (uint32_t)idt_descriptor;

    for(int i=0 ; i < TOTAL_NO_OF_INTERRUPT ; i++)
    {
        idt_set(i , no_interrupt);
    }
    // keyboard interrupt handler
    idt_set(0x21,int21h);
    idt_load(&idtr_descriptor);

    enable_interrupt();
}


/*
    Interrupt Service Routine
*/
void int21h_handler(){
    // handling interrupt
    print("Keyboard Pressed..");

    // send ACK to PIC we finished handling the interrupt request
    outb(0x20,0x20);
}
void no_interrupt_handler(){
    // handling interrupt

    // TODO:....

    // send ACK to PIC we finished handling the interrupt request
    outb(0x20,0x20);    
}
