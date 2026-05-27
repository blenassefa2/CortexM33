// include/timing/watchdog_timer.h
#ifndef WATCHDOG_TIMER_H
#define WATCHDOG_TIMER_H

#include <stdint.h>

// Watchdog base and registers
#define WATCHDOG_BASE       0x40058000UL
#define WATCHDOG_CTRL       (*((volatile uint32_t *)(WATCHDOG_BASE + 0x000)))
#define WATCHDOG_LOAD       (*((volatile uint32_t *)(WATCHDOG_BASE + 0x004)))
#define WATCHDOG_TICK       (*((volatile uint32_t *)(WATCHDOG_BASE + 0x02C)))

// TICK register bits
#define TICK_ENABLE         (1u << 9)
#define TICK_CYCLES         12u          // 12 clk_ref cycles = 1 µs tick (12MHz XOSC)

// Load value — use max safe value
// Due to errata RP2040-E1 counter decrements TWICE per tick,
// so real max is 0xFFFFFF / 2 = ~8.3 seconds worth of ticks
#define WDT_LOAD_MAX        0xFFFFFFUL

// CTRL register bits
#define CTRL_ENABLE         (1u << 30)
#define CTRL_TRIGGER        (1u << 31)   // writing 1 resets the chip — never set this

void     watchdog_timer_init(void);
uint32_t watchdog_timer_start(void);
uint32_t watchdog_timer_stop(uint32_t start_val);  // returns ticks elapsed
uint32_t watchdog_ticks_to_us(uint32_t ticks);     // convert ticks to microseconds

#endif