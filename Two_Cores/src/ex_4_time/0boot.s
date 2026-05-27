.syntax unified
.arch   armv6-m

.section .vectors, "a", %progbits
.align   2
.globl   __Vectors
__Vectors:
        .long  __StackTop_CPU0
        .long  Reset_Handler
        .long  0
        .long  HardFault_Handler
        .long  0

.thumb
.section .text
.align  2

.thumb_func
.globl  Reset_Handler
Reset_Handler:
        ldr  r0, =__Vectors
        ldr  r1, =0xE000ED08
        str  r0, [r1]

        ldr  r0, =__StackTop_CPU0
        msr  msp, r0

        /* zero .bss */
        ldr  r0, =_bss_start
        ldr  r1, =_bss_end
        movs r2, #0
bss_loop:
        cmp  r0, r1
        bge  bss_done
        str  r2, [r0]
        adds r0, r0, #4
        b    bss_loop
bss_done:

        /* copy .data from flash to SRAM */
        ldr  r0, =_data_start
        ldr  r1, =_data_end
        ldr  r2, =_data_load
data_loop:
        cmp  r0, r1
        bge  data_done
        ldr  r3, [r2]
        str  r3, [r0]
        adds r0, r0, #4
        adds r2, r2, #4
        b    data_loop
data_done:

        bl   main 

.thumb_func
.weak   HardFault_Handler
HardFault_Handler:
        b    .
.end
