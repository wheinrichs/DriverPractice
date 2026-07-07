#include <stdint.h>

// Write a function that sets multiple bits in a register at once
// Why static inline? Static means that this function can be used in multiple files, inline is similar to macro but with type checking
// mask tells us which values to clear, shift is how much we need to move the value to write the bits, and value is what to write
static inline void write_field(volatile uint32_t *reg, uint32_t mask, uint8_t shift, uint32_t value) {
    if(reg == nullptr) {
        return;
    }
    
    // Clear the bit
    *reg &= ~mask;
    *reg |= value << shift;
}