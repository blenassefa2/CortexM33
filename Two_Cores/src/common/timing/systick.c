// systick.c


#include "../../../include/timing/systick.h"

void systick_init(void)
{
    SYSTICK_LOAD = 0x00FFFFFF;
    SYSTICK_VAL  = 0;
    SYSTICK_CTRL = 0x5;
}

uint32_t systick_elapsed(uint32_t start, uint32_t end)
{
    return (start - end) & 0x00FFFFFF;
}