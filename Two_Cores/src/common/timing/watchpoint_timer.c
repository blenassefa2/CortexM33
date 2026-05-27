
#include "../../../include/timing/watchdog_timer.h"




void watchdog_timer_init(void) {
    
    WATCHDOG_TICK = TICK_CYCLES | TICK_ENABLE;

    while (!(WATCHDOG_TICK & (1u << 10)));
    
    WATCHDOG_LOAD = WDT_LOAD_MAX;
    WATCHDOG_CTRL = CTRL_ENABLE;

    volatile uint32_t delay = 100;
    while (delay--);
}


uint32_t watchdog_timer_start(void) {
    
    WATCHDOG_LOAD = WDT_LOAD_MAX;

    
    return WATCHDOG_CTRL & 0x00FFFFFFu;
}

uint32_t watchdog_timer_stop(uint32_t start_val) {
    uint32_t current = WATCHDOG_CTRL & 0x00FFFFFFu;

  
    if (current > start_val) {
        
        return 0xFFFFFFFFu;
    }

    uint32_t raw_ticks = start_val - current;


    return raw_ticks / 2u;
}


uint32_t watchdog_ticks_to_us(uint32_t ticks) {
    return ticks;   // 1 tick = 1 µs, already compensated in stop()
}