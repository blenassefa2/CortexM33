MEMORY
{
    FLASH (rx)  : ORIGIN = 0x10000000, LENGTH = 2048K
    SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 264K
}
ENTRY(Reset_Handler)
SECTIONS
{

    .vectors 0x10000000 : 
    {
        KEEP(*(.vectors))
    } > FLASH

   
    .boot :
    {
        KEEP(*0boot.o(.text .text.*))
    } > FLASH

    
    /* I want to keep this block in the flash to do execute from flash and use the cache*/
    .victim_text 0x10001000 :
    {
          *(.victim_text .victim_text.*)
    } > FLASH

   
   
   
    .text :
    {
        *(.text .text.*)
    } > FLASH

   
    .rodata :
    {
        *(.rodata .rodata.*)
    } > FLASH

    
    .data :
    {
        _data_load  = LOADADDR(.data);
        _data_start = .;
        *(.data .data.*)
        _data_end   = .;
    } > SRAM AT > FLASH

    
    .bss :
    {
        _bss_start = .;
        *(.bss .bss.* COMMON)
        _bss_end   = .;
    } > SRAM

    
    __StackTop_CPU0 = ORIGIN(SRAM) + LENGTH(SRAM);
}