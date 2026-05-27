#include "../../include/timing/systick.h"

#define REPEATS 1000
// since all of the code in this cpu0.c file is stored in the flash (forced by linker script),
//  we'll be executing from flash and use XIP cache
__attribute__((noinline, section(".victim_text")))   static uint32_t additional_block(uint32_t x) {
    // Keep this instruction-heavy so accesses are dominated by XIP fetches.
    x ^= 0xA5A5A5A5u;
    x = (x << 3) | (x >> 29);
    x += 0x1234u;
    __asm volatile ("nop\n nop\n nop\n nop");
    return x;
}

__attribute__((noinline))
void flush_icache(void) {
    asm volatile(
        ".align 6\n"

        "1:\n"
        ".rept 8192\n"
        "nop\n"
        ".endr\n"

        :
        :
        : "memory"
    );
}

/**
* This is the function I want to check if it is executed or not executed
*/
__attribute__((section(".victim_text"))) void foo(void) {
    volatile uint32_t sink = 0;
    for (int i = 0; i < 64; i++) {
        sink ^= additional_block((uint32_t)i);
    }
    (void)sink;
}

/**

variable naming:
#-of-times_cache-state_iteration_time


cache states cold, flushed and disabled should be equivalent/somewhat similar

cache state warm should be significantly lower than all the three above

 */

uint32_t empty_iterations_time = -1;

uint32_t single_cold_iteration_time = -1;
uint32_t single_warm_iteration_time = -1;

uint32_t single_flushed_iteration_time = -1;

uint32_t single_disabled_iteration_time = - 1;


uint32_t s, e = -1;
void core0_main(void) {

    systick_init();
    enable_xip_cache(); 

    
    
    s = SYSTICK_VAL;
    foo();
    e = SYSTICK_VAL;
    single_cold_iteration_time = systick_elapsed(s, e); 
    
    s = SYSTICK_VAL;
    foo();
    e = SYSTICK_VAL;
    single_warm_iteration_time = systick_elapsed(s, e); 

    flush_icache();

    s = SYSTICK_VAL;
    foo();
    e = SYSTICK_VAL;
    single_flushed_iteration_time = systick_elapsed(s, e); 


    disable_xip_cache();

    s = SYSTICK_VAL;
    foo();
    e = SYSTICK_VAL;
    single_disabled_iteration_time = systick_elapsed(s, e);

    // core0 parked
    while(1);
}

int main(void) {
    core0_main();
    while (1);
}

