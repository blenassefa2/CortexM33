#include <stdint.h>

#define SIO_BASE        0xD0000000
#define SIO_GPIO_OUT    (*(volatile uint32_t *)(SIO_BASE + 0x10))
#define SIO_GPIO_OE     (*(volatile uint32_t *)(SIO_BASE + 0x20))

#define IO_BANK0_BASE   0x40014000
#define GPIO_CTRL(n)    (*(volatile uint32_t *)(IO_BANK0_BASE + 0x04 + (n) * 8))

#define PADS_BANK0_BASE 0x4001C000
#define GPIO_PAD(n)     (*(volatile uint32_t *)(PADS_BANK0_BASE + 0x04 + (n) * 4))

#define RESETS_BASE      0x4000C000
#define RESETS_RESET     (*(volatile uint32_t *)(RESETS_BASE + 0x00))
#define RESETS_RESET_DONE (*(volatile uint32_t *)(RESETS_BASE + 0x08))
#define RESET_IO_BANK0   (1u << 5)
#define RESET_PADS_BANK0 (1u << 8)

#define LED_PIN 25

// Simple busy delay
void delay(volatile uint32_t count) {
    while (count--) {
        __asm volatile ("nop");
    }
}

#define LED_PIN 25

void blink_main(void) {
    // If we jump straight into firmware via debugger, these can still be held in reset.
    RESETS_RESET &= ~(RESET_IO_BANK0 | RESET_PADS_BANK0);
    while ((RESETS_RESET_DONE & (RESET_IO_BANK0 | RESET_PADS_BANK0)) !=
           (RESET_IO_BANK0 | RESET_PADS_BANK0)) {}

    // Set function to SIO
    GPIO_CTRL(LED_PIN) &= ~0x1F;
    GPIO_CTRL(LED_PIN) |= 5;

    // Enable output
    SIO_GPIO_OE |= (1 << LED_PIN);

    while (1) {
        SIO_GPIO_OUT ^= (1 << LED_PIN);  // toggle
        delay(500000);
    }
}

int main(void) {
    // uint32_t stack_top = (uint32_t)(core1_stack + 256); // top of array

    // core1_launch(__Vectors, stack_top, core1_main);

    blink_main();
    while (1);
}