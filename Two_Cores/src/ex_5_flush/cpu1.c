// cpu1.c  — spy core (core 1)

#include "../../include/shared.h"
// #include "../../include/timing/systick.h"

#include <stdatomic.h>


extern void foo();

void core1_main(void)
{
   
    // systick_init();
    
    atomic_store_explicit(&cpu1_running, true, memory_order_release);
   

    atomic_store_explicit(&cpu1_running, false, memory_order_release);
   
    while (1);  // park core 1
}