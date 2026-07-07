#include <stdint.h>
#include <stdbool.h>

typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t IDR; 
    volatile uint32_t ODR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
}gpio_regs_t;

typedef enum {
    INPUT = 0,
    OUTPUT = 1,
    ALTERNATE_FUNCTION = 2,
    ANALOG = 3
}gpio_mode_t;

void gpio_set_mode(volatile gpio_regs_t *reg, uint8_t pin, gpio_mode_t mode);

bool gpio_read_pin(volatile gpio_regs_t *reg, uint8_t pin);

void gpio_write_pin(volatile gpio_regs_t *reg, uint8_t pin, bool value);

void gpio_set_alternate_function(volatile gpio_regs_t *reg, uint8_t pin, uint8_t alt_function);