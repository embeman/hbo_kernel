section .asm

global idt_load

global enable_interrupt
global disable_intterupt

extern int21h_handler
extern no_interrupt_handler

global int21h
global no_interrupt

idt_load:
    push ebp
    mov ebp , esp
    mov ebx , [ebp + 8]
    lidt [ebx]
    pop ebp
    ret

enable_interrupt:
    sti
    ret

disable_intterupt:
    cli 
    ret

int21h:
    cli
    pushad 
    call int21h_handler
    popad
    sti
    iret

no_interrupt:
    cli
    pushad 
    call no_interrupt_handler
    popad
    sti
    iret
