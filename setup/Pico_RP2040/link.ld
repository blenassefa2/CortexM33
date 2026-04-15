MEMORY
{
    FLASH (rx)  : ORIGIN = 0x10000000, LENGTH = 2048K
    SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 264K
}
ENTRY(Reset_Handler)
SECTIONS
{
    /* Vector table at start of flash */
    .vectors 0x10000000 : 
    {
        KEEP(*(.vectors))
    } > FLASH

    /* Boot code immediately after vector table, don't overlap with .victim_text explicitly */
    .boot :
    {
        KEEP(*0boot.o(.text .text.*))
    } > FLASH

    .blink_text 0x10001000 :
    {
        *blink.o(.text .text.*)
    } > FLASH

    /* Exclude sections just assigned (above) from general .text region below */
    .text :
    {
        /* Exclude .vectors, .boot, .victim_text, .spy_evict from .text catch-all */
        *(.text .text.*)
        EXCLUDE_FILE(*0boot.o *blink.o) *(.text .text.*)
    } > FLASH

    /* Read-only data */
    .rodata :
    {
        *(.rodata .rodata.*)
    } > FLASH

    /* Initialized data in SRAM, loaded from flash */
    .data :
    {
        _data_load  = LOADADDR(.data);
        _data_start = .;
        *(.data .data.*)
        _data_end   = .;
    } > SRAM AT > FLASH

    /* Uninitialized data */
    .bss :
    {
        _bss_start = .;
        *(.bss .bss.* COMMON)
        _bss_end   = .;
    } > SRAM

    /* core 0 stack grows down from top of SRAM */
    __StackTop_CPU0 = ORIGIN(SRAM) + LENGTH(SRAM);
}