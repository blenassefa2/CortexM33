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


uint32_t empty_iterations_time = -1;
uint32_t single_first_iteration_time = -1;
uint32_t cached_iterations_time = -1;
uint32_t uncached_single_iteration_time = -1;


uint32_t s, e = -1;
void core0_main(void) {

    systick_init();
    enable_xip_cache(); 

    
    s = SYSTICK_VAL;
   
    for (int i = 0; i < REPEATS; i++) {
        ; //no operation
    }
    e = SYSTICK_VAL;
    empty_iterations_time = systick_elapsed(s, e);
    
    s = SYSTICK_VAL;
   
    foo();
    e = SYSTICK_VAL;
    single_first_iteration_time = systick_elapsed(s, e); // could be affected by prefetch

    s = SYSTICK_VAL;
    for(int i = 0; i < REPEATS; i++){
        foo();    
    }
    e = SYSTICK_VAL;
       
    cached_iterations_time = systick_elapsed(s, e); // Lower bound

    disable_xip_cache();
    s = SYSTICK_VAL;
    foo();
    e = SYSTICK_VAL;
    uncached_single_iteration_time = systick_elapsed(s, e); // Higher bound

   
    // core0 parked
    while(1);
}

int main(void) {
    core0_main();
    while (1);
}