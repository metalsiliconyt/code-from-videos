/* Minimal Linker Script for Video Demo */
MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 128K
    RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS
{
    /* Code and Constants go to Flash */
    .text : { *(.text) } > FLASH
    .rodata : { *(.rodata*) } > FLASH

    /* Initialized data goes to RAM */
    .data : { *(.data) } > RAM AT > FLASH
    
    /* Uninitialized data (zeroed) goes to RAM */
    .bss : { *(.bss) } > RAM
}