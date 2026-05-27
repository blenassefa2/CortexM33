#ifndef SHARED_H
#define SHARED_H


#include <stdint.h>
#include <stdatomic.h>
#include <stdbool.h>


#define ITERATIONS 4100



extern atomic_uint_fast32_t shared_counter;

extern volatile atomic_bool cpu1_running;
extern volatile atomic_bool cpu0_running;

#endif