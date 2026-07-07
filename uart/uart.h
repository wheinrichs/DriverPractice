#include <stdint.h>
#include "ring_buffer.h"
#include "status.h"
#include "timebase.h"

#define UART_SR_RXNE    (1U << 5)
#define UART_SR_TXE     (1U << 7)
#define UART_SR_ORE     (1U << 3)

#define UART_CR1_UE     (1U << 13)
#define UART_CR1_RE     (1U << 2)
#define UART_CR1_TE     (1U << 3)
#define UART_CR1_RXNEIE (1U << 5)

typedef struct{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t CR1;
    volatile uint32_t BRR;
}uart_regs_t;

typedef struct{
    uart_regs_t *regs;
    ring_buffer_t rx_buffer;
}uart_driver_t;

driver_status_t init(uart_driver_t *uart, uart_regs_t* regs, uint32_t baud_divisor);
driver_status_t write_byte(uart_driver_t *uart, uint8_t byte, uint32_t timeout_ms);
