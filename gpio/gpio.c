#include "gpio.h"
#include <stdbool.h>
#include <stdlib.h>

void gpio_set_mode(volatile gpio_regs_t *reg, uint8_t pin, gpio_mode_t mode) {
    if(reg == NULL) {
        return;
    }

    reg->MODER &= ~(3U << (2U * pin));
    reg->MODER |= (mode << (2U * pin));
    return;
}

bool gpio_read_pin(volatile gpio_regs_t *reg, uint8_t pin) {
    if(reg == NULL) {
        return false;
    }
    return (reg->IDR & (1U << (pin))) != 0U;
}

void gpio_write_pin(volatile gpio_regs_t *reg, uint8_t pin, bool value) {
    if (reg == NULL) {
        return;
    }
    reg->ODR &= ~(1U << pin);
    if(value) {
        reg->ODR |= (1U << pin);
    }
    return;
}

void gpio_set_alternate_function(volatile gpio_regs_t *reg, uint8_t pin, uint8_t alt_function) {
    if (reg == NULL) {
        return;
    }
    if(pin < 8) {
        reg->AFRL &= ~(0xFU << (4 * pin));
        reg->AFRL |= (alt_function << (4 * pin));
    }
    else {
        reg->AFRH &= ~(0XFU << (4 * (pin % 8)));
        reg->AFRH |= (alt_function << (4 * (pin % 8)));
    }
    return;
}