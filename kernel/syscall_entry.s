.global syscall_handler_entry
.extern serial_print

.section .text
syscall_handler_entry:
    /* RDI = syscall number / args */
    sysretq
