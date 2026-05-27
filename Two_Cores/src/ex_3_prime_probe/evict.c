

#include <stdint.h>

// 16KB: one full sweep primes both ways for all 1024 sets
const uint8_t __attribute__((section(".spy_evict")))
    evict_buf[16384] = { [0 ... 16383] = 0xAB };