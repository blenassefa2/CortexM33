#ifndef MULTICORE_H
#define MULTICORE_H

#include <stdint.h>


// for rp2040 platform it uses mailbox system to notify eachother
void core1_launch(uint32_t *vector_table,
    uint32_t  stack_top,
    void     (*entry_point)(void));

#endif