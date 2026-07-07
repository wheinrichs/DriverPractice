#include "uart.h"

driver_status_t uart_init(uart_driver_t *uart, uart_regs_t* regs, uint32_t baud_divisor) {
    if(uart == NULL || regs == NULL) {
        return DRIVER_ERR_INVALID_ARG;
    }
    
    rb_init(&uart->rx_buffer);

    regs->BRR = baud_divisor;
    regs->CR1 |= UART_CR1_RE;
    regs->CR1 |= UART_CR1_TE;
    regs->CR1 |= UART_CR1_UE;

    uart->regs = regs;

    return DRIVER_OK;
}

driver_status_t uart_write_byte(uart_driver_t *uart, uint8_t byte, uint32_t timeout_ms) {
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

driver_status_t uart_write_buffer(uart_driver_t *uart, const uint8_t *buffer, uint32_t len, uint32_t timeout_ms) {
    if(buffer == NULL) {
        return DRIVER_ERR_INVALID_ARG;
    }
    for(uint32_t i = 0; i < len; i ++) {
        driver_status_t res = write_byte(uart, buffer[i], timeout_ms);
        if(res != DRIVER_OK) {
            return res;
        }
    }
    return DRIVER_OK;
}

void uart_IRQ_handler(uart_driver_t *uart) {
    if(uart == NULL) {
        return;
    }

    if(uart->regs->SR & UART_SR_RXNE != 0U) {
        rb_push(&uart->rx_buffer, (uart->regs->DR & 0xFFU));
    }
}

driver_status_t read_byte(uart_driver_t *uart, uint8_t *out) {
    if(uart == NULL || out == NULL) {
        return DRIVER_ERR_INVALID_ARG;
    }
    if(rb_is_empty(&uart->rx_buffer)) {
        return DRIVER_ERR_EMPTY;
    }
    rb_pop(&uart->rx_buffer, out);
    return DRIVER_OK;
}