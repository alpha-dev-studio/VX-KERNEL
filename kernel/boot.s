.global _start
.extern kmain

.section .text
_start:
    /* Set up stack */
    mov $stack_top, %rsp
    
    /* Align stack to 16 bytes for System V AMD64 ABI */
    and $~15, %rsp

    /* Call C kernel main */
    call kmain

    /* Halt if returned */
    cli
1:  hlt
    jmp 1b

.section .bss
.align 16
stack_bottom:
    .skip 16384 /* 16 KiB stack */
stack_top:

.section .note.GNU-stack,"",@progbits
