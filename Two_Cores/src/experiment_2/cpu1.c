#include "../../include/print.h"
#include "../../include/shared.h"
#include "../../include/mpu.h"
#include "../../include/timing/systick.h"
#include "../../include/platform/an521.h"


#define PROBE_ADDR 0x28208000 // A different address in shared RAM (NS_RAM)

#define SPY_DUMP_COUNT 4096
uint32_t spy_results[SPY_DUMP_COUNT];



void interrupt_here(void){
    return;
}

void main_cpu1(void)
{
    sh_puts("- cpu #1 main()\n");

    mpu_setup();
    systick_init();
    // Enable DWT Cycle Counter for time analysis
   
    sh_puts("- cpu #1  mpu setup done\n");

    

    uint32_t min_ticks = 0xFFFFFFFF;
    uint32_t max_ticks = 0;
    uint32_t total     = 0;
    uint32_t contended = 0; // iterations that took suspiciously long

    sh_puts("- cpu #1 running \n");
    cpu1_running = true;

    while (!cpu0_running) ;
    
    // uint32_t retries = 0;

    // for (int i = 0; i < ITERATIONS; i++)
    // {
    //     uint32_t attempts = 0;
    //     int64_t tmp;
    //     int status;
    
    //     do {
    //         __asm__ volatile (
    //             "ldaex  %0, [%2]    \n"
    //             "add    %0, %0, #1  \n"
    //             "stlex  %1, %0, [%2]\n"
    //             : "=&r"(tmp), "=&r"(status)
    //             : "r"(&shared_counter)
    //             : "memory"
    //         );
    //         attempts++;
    //     } while (status != 0);
    
    //     retries += (attempts - 1); // only count the failed ones
    // }
    
    // sh_puts("retries: "); sh_putx(retries); sh_puts("\n");
    for (int i = 0; i < ITERATIONS; i++)
    {
        uint32_t s = SYSTICK_VAL;
        shared_counter++;
        uint32_t e = SYSTICK_VAL;
        if (i < SPY_DUMP_COUNT) {
            spy_results[i] = systick_elapsed(s, e);
        }

        // if (elapsed < min_ticks) min_ticks = elapsed;
        // if (elapsed > max_ticks) max_ticks = elapsed;
        // total += elapsed;
        // if (elapsed > 10) contended++; // tune threshold after first run
    }

    // sh_puts("min    : "); sh_putx(min_ticks); sh_puts("\n");
    // sh_puts("max    : "); sh_putx(max_ticks); sh_puts("\n");
    // sh_puts("total  : "); sh_putx(total);     sh_puts("\n");
    // sh_puts("avg    : "); sh_putx(total / ITERATIONS); sh_puts("\n");
    // sh_puts("contended: "); sh_putx(contended); sh_puts("\n");
       
    
    // sh_puts("cpu1 total ticks : "); sh_putx(total); sh_puts("\n");
    sh_puts("cpu1 done waiting for cpu0\n");
    while (cpu0_running);

    int i = 0;
    while (i < SPY_DUMP_COUNT)
    {
        sh_putx(spy_results[i]);
        sh_puts("\n");
        i++;
    }

 

    while (true);
}