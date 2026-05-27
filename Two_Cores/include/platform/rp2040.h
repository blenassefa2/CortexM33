#ifndef RP2040_H
#define RP2040_H
#include <stdint.h>

// ── SysTick (standard Cortex-M, same addresses as before) ───────
#define SYSTICK_CTRL  (*((volatile uint32_t *)0xE000E010))
#define SYSTICK_LOAD  (*((volatile uint32_t *)0xE000E014))
#define SYSTICK_VAL   (*((volatile uint32_t *)0xE000E018))

// ── SIO (Single-cycle IO) ────────────────────────────────────────
#define SIO_BASE              0xD0000000UL
#define SIO_CPUID             (*((volatile uint32_t *)(SIO_BASE + 0x000)))

// FIFO — used for inter-core mailbox / core1 launch
#define SIO_FIFO_ST           (*((volatile uint32_t *)(SIO_BASE + 0x050)))
#define SIO_FIFO_WR           (*((volatile uint32_t *)(SIO_BASE + 0x054)))
#define SIO_FIFO_RD           (*((volatile uint32_t *)(SIO_BASE + 0x058)))

// FIFO status bits
#define FIFO_ST_VLD           (1u << 0)   // RX FIFO has data
#define FIFO_ST_RDY           (1u << 1)   // TX FIFO has space
#define FIFO_ST_ROE           (1u << 2)   // RX overflow error — clear by writing
#define FIFO_ST_WOF           (1u << 3)   // TX underflow error


// Timer registesrs and control
#define TIMER_BASE  0x40054000UL
#define TIMELR      (*((volatile uint32_t *)(TIMER_BASE + 0x0C)))  // read this first
#define TIMEHR      (*((volatile uint32_t *)(TIMER_BASE + 0x08)))  // latches on TIMELR read


// Cache control
#define XIP_CTRL_BASE 0x14000000

#define XIP_CTRL_CTRL  (*(volatile uint32_t *)(XIP_CTRL_BASE + 0x00))

#define XIP_CTRL_FLUSH (*(volatile uint32_t *)(XIP_CTRL_BASE + 0x04))

#define XIP_CTRL_STAT  (*(volatile uint32_t *)(XIP_CTRL_BASE + 0x08))

#define XIP_CTRL_EN            (1u << 0)

#define XIP_STAT_FLUSH_READY   (1u << 0)

__attribute__((section(".time_critical"), noinline))
static void flush_xip_cache(void){

    __asm volatile ("dsb");

    XIP_CTRL_FLUSH = 1;

    while (!(XIP_CTRL_STAT & XIP_STAT_FLUSH_READY)) {

    }

    __asm volatile ("dsb");

    __asm volatile ("isb");

}

static void enable_xip_cache(void) {

    XIP_CTRL_CTRL |= XIP_CTRL_EN;

//    flush_xip_cache();
}

static void disable_xip_cache(void) {
    XIP_CTRL_CTRL &= ~XIP_CTRL_EN;
}


#endif /* RP2040_H */