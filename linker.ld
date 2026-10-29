OUTPUT_FORMAT(elf64-x86-64)
ENTRY(kmain)

PHDRS
{
    load PT_LOAD FLAGS((1 << 0) | (1 << 1) | (1 << 2));
}

SECTIONS
{
    . = 0xffffffff80000000;

    .text : ALIGN(4096) {
        *(.text .text.*)
    } :load

    .rodata : ALIGN(4096) {
        *(.rodata .rodata.*)
    } :load

    .data : ALIGN(4096) {
        *(.data .data.*)
    } :load

    .bss : ALIGN(4096) {
        *(.bss .bss.*)
        *(COMMON)
    } :load

    /DISCARD/ : {
        *(.eh_frame*)
        *(.note .note.*)
        *(.got .got.*)
    }
}
