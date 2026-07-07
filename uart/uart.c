#include "uart.h"

driver_status_t init(uart_driver_t *uart, uart_regs_t* regs, uint32_t baud_divisor) {
    if(uart == NULL || regs == NULL) {
        return DRIVER_ERR_INVALID_ARG;
    }
    
    rb_init(&uart->rx_buffer);

    regs->BRR = baud_divisor;
    regs->CR1 &= UART_CR1_RE;
    regs->CR1 &= UART_CR1_TE;
    regs->CR1 &= UART_CR1_UE;

    uart->regs = regs;

    return DRIVER_OK;
}

driver_status_t write_byte(uart_driver_t *uart, uint8_t byte, uint32_t timeout_ms) {
    if(uart == NULL) {
        return DRIVER_ERR_INVALID_ARG;
    }

    uint32_t start = millis();

    while((uart->regs->SR & UART_SR_TXE) == 0U) {
        if(millis() - start > timeout_ms) {
            return DRIVER_ERR_TIMEOUT;
        }
    }

    uart->regs->DR = byte;
    return DRIVER_OK;
}