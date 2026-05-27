

#include "../../include/mpu.h"
#include "../../include/print.h"
#include "../../include/shared.h"
#include "../../include/timing/systick.h"
#include "../../include/multicore/cpu_control.h"

#include <stdbool.h>




uint32_t secret_bit = 0;
void main_cpu0(void)
{
    sh_puts("- mpu setup()\n");
    mpu_setup();
    systick_init();

    sh_puts("- cpu #1 setup()\n");
    setup_cpu1();


   
    cpu0_running = true;
    sh_puts("- cpu #0 running \n");
    while (!cpu1_running);

    for (int i = 0; i < ITERATIONS; i++)
    {
        if (secret_bit == 0)
        {
            __asm volatile ("nop\n nop\n nop\n nop");
            secret_bit = 1;
        }
        else
        {
            __asm volatile ("nop\n nop\n nop\n nop");
            secret_bit = 0;
        }
    
    }

    cpu0_running = false;
    while (cpu1_running);

    // sh_puts("cpu0: ");
    // sh_putx(shared_counter);
    // sh_puts("\n");

    while (true);
}



// Read fault status registers and loop forever

