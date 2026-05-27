#include "../../include/shared.h"
#include "../../include/multicore/multicore.h"

// core 1 stack in SRAM — 1KB is plenty for the spy
static uint32_t core1_stack[256];   // 256 × 4 = 1024 bytes

// defined in cpu1.c
extern void core1_main(void);

// vector table from linker
extern uint32_t __Vectors[];

uint32_t secret_trace[ITERATIONS];

__attribute__((noinline)) static uint32_t victim_block(uint32_t x) {
    // Keep this instruction-heavy so accesses are dominated by XIP fetches.
    x ^= 0xA5A5A5A5u;
    x = (x << 3) | (x >> 29);
    x += 0x1234u;
    __asm volatile ("nop\n nop\n nop\n nop");
    return x;
}

void foo(void) {
    volatile uint32_t sink = 0;
    for (int i = 0; i < 64; i++) {
        sink ^= victim_block((uint32_t)i);
    }
    (void)sink;
}


void core0_main(void) {
   

    atomic_store(&cpu0_running, true);
    while (!atomic_load(&cpu1_running));  

    for (int i = 0; i < ITERATIONS; i++) {
        int secret_bit = (i & 1);
        secret_trace[i] = (uint32_t)secret_bit;
        if (secret_bit == 1) {
            foo();
        }
    }
    atomic_store(&cpu0_running, false);
}
int main(void) {
    uint32_t stack_top = (uint32_t)(core1_stack + 256);

    core1_launch(__Vectors, stack_top, core1_main);

    core0_main();
    while (1);
}