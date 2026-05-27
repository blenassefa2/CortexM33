#include "../../../include/multicore/multicore.h"
#include "../../../include/platform/rp2040.h"

// ── helpers ──────────────────────────────────────────────────────

static void fifo_drain(void)
{
    // discard anything sitting in the RX FIFO
    while (SIO_FIFO_ST & FIFO_ST_VLD)
        (void)SIO_FIFO_RD;
    // clear error flags
    SIO_FIFO_ST = FIFO_ST_ROE | FIFO_ST_WOF;
}

static void fifo_push(uint32_t val)
{
    // wait until TX FIFO has space
    while (!(SIO_FIFO_ST & FIFO_ST_RDY));
    SIO_FIFO_WR = val;
    // SEV so core 1 wakes from WFE
    __asm volatile ("sev");
}

static uint32_t fifo_pop(void)
{
    // wait until RX FIFO has data
    while (!(SIO_FIFO_ST & FIFO_ST_VLD))
        __asm volatile ("wfe");
    return SIO_FIFO_RD;
}

static void fifo_push_blocking(uint32_t val)
{
    fifo_push(val);
    // block until core 1 echoes the value back
    uint32_t echo = fifo_pop();
    (void)echo;   // you can assert echo == val here if you want
}

// ── public API ───────────────────────────────────────────────────

void core1_launch(uint32_t *vector_table,
                  uint32_t  stack_top,
                  void     (*entry_point)(void))
{
    // The required sequence (RP2040 datasheet §2.8.2):
    //   send 0        — resets core 1 state machine
    //   send 0        — second flush
    //   send 1        — tells core 1 a real launch sequence follows
    //   send vtor     — vector table address
    //   send sp       — initial stack pointer
    //   send entry    — entry point (core 1 jumps here and echoes, then runs)

    uint32_t launch_cmd[] = {
        0,
        0,
        1,
        (uint32_t)vector_table,
        stack_top,
        (uint32_t)entry_point
    };

    for (int i = 0; i < 6; i++) {
        if (launch_cmd[i] == 0) {
            // For the two zeros: push without waiting for echo
            // because core 1 may not echo the zeros if it's already
            // in the middle of a previous sequence — just drain instead
            fifo_drain();
            fifo_push(0);
        } else {
            fifo_push_blocking(launch_cmd[i]);
        }
    }
}

