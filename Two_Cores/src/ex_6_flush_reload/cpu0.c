#include "../../include/timing/systick.h"

#include "../../include/multicore/multicore.h"
#include <stdint.h>
#include "../../include/shared.h"

static uint32_t core1_stack[256];   // 256 × 4 = 1024 bytes

// defined in cpu1.c
extern void core1_main(void);

// vector table from linker
extern uint32_t __Vectors[];

#define REPEATS 10000
#define SECRET 6
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

void not_important(){
    __asm volatile ("nop\n nop\n nop\n nop");
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

extern uint32_t foo_time[4600];

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



void core0_main(void) {
   
    int counter  = 0;
    for (int i = 0; i < REPEATS; i++){
       
        if (counter == 0)
        {
            foo();
        }
        else
        {
            not_important();
        }

        counter++;

        if (counter == SECRET) {

            counter = 0;

        }
    }
   
    // park core 0
    while(1);
}

int main(void) {

    uint32_t stack_top = (uint32_t)(core1_stack + 256);
    
    core1_launch(__Vectors, stack_top, core1_main);
    enable_xip_cache(); 
  
    core0_main();
   
    while (1);
}

