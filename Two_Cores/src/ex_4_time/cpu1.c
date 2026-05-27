// cpu1.c  — spy core (core 1)

#include "../../include/shared.h"
// #include "../../include/timing/systick.h"

#include <stdatomic.h>




//  timing storage 
// Per-set timing


void interrupt_here(void){
    return;
}

extern void foo();
// core 1 entry 
void core1_main(void)
{
   
    // systick_init();
    

    // sh_puts("core1_main running\n");
    atomic_store_explicit(&cpu1_running, true, memory_order_release);
   

    atomic_store_explicit(&cpu1_running, false, memory_order_release);
    // sh_puts("core1_main finished\n");
    while (1);  // park core 1
}