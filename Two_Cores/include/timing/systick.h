#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>
#include "../platform/rp2040.h"
// Currently I only have systick but Debug watchpoint and Trace Unit is much more fast

void systick_init(void);
uint32_t systick_elapsed(uint32_t start, uint32_t end);


#endif