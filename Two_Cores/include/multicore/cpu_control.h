#ifndef CPU_CONTROL_H
#define CPU_CONTROL_H

#include <stdint.h>
extern uint32_t __Vectors;

// Initially built for cortex-m33 I didn't bother to change the file name so 
// I stayed with cpu_control.h and cpu_control.c for cortex-m33
// and multicore.h and multicore.c for cortex-m0+  RP2040 platform

uint32_t get_cpu_id(void);
void setup_cpu1();

#endif