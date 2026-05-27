// core1.c  — spy core (core 1)

#include "../../include/shared.h"
#include "../../include/timing/systick.h"



#define  WAIT_WINDOW 1000
#define  SPY_TIME 4960

#define THRESHOLD 7000

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

uint32_t foo_time[SPY_TIME];
uint32_t foo_count;

extern void foo();

uint32_t s, e = -1;

void core1_main(void)
{
    
    systick_init();
    
    // atomic_store_explicit(&cpu1_running, true, memory_order_release);
    int counter = 0;
    while(counter < SPY_TIME){
    
        flush_icache();
        
        for(int i = 0; i < WAIT_WINDOW; i++);

        s = SYSTICK_VAL;
        foo();
        e = SYSTICK_VAL;
        foo_time[counter] = systick_elapsed(s, e); 
        if (foo_time[counter] < THRESHOLD)
            foo_count++;

        counter++;
    
    }

    // atomic_store_explicit(&cpu1_running, false, memory_order_release);
   
    while (1);  // park core 1
}