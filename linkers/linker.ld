/* 1. Define the Entry Point */
ENTRY(Reset_Handler)

/* 2. Define Physical Memory Blocks */
MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 128K
    RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 20K
}

/* 3. Define Output Sections */
SECTIONS
{
    /* The Program Code goes into FLASH */
    .text :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector)) /* Interrupt vector table - MUST be first */
        *(.text)             /* Application code */
        *(.rodata)           /* Read-only data (constants) */
        . = ALIGN(4);
    } > FLASH

    /* Used for initializing .data in RAM later */
    _sidata = LOADADDR(.data);

    /* Initialized data goes into RAM, but is stored in FLASH */
    .data : 
    {
        . = ALIGN(4);
        _sdata = .;        /* Create a symbol for the start of data */
        *(.data)           /* Variables like: int x = 5; */
        . = ALIGN(4);
        _edata = .;        /* Create a symbol for the end of data */
    } > RAM AT > FLASH

    /* Uninitialized data (zero-filled) */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;         /* Symbol for start of BSS */
        *(.bss)            /* Variables like: int y; */
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;         /* Symbol for end of BSS */
    } > RAM

    /* Stack section - usually at the end of RAM */
    ._user_stack :
    {
        . = ALIGN(8);
        . = . + 0x400;     /* Reserve 1KB for stack */
        _estack = .;
    } > RAM
}