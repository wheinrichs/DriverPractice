#include "uart/uart.h"
#include <stdio.h>
#include <stdint.h>

static int tests_run = 0;
static int tests_failed = 0;

#define TEST_ASSERT(cond) do { \
    tests_run++; \
    if (!(cond)) { \
        tests_failed++; \
        printf("FAIL: %s:%d: %s\n", __FILE__, __LINE__, #cond); \
    } \
} while (0)

/*
    These tests assume your fake UART register struct has fields like:

    typedef struct {
        volatile uint32_t SR;
        volatile uint32_t DR;
        volatile uint32_t BRR;
        volatile uint32_t CR1;
    } uart_regs_t;

    And bit masks like:

    #define USART_SR_TXE     ...
    #define USART_SR_RXNE    ...
    #define USART_CR1_TE     ...
    #define USART_CR1_RE     ...
    #define USART_CR1_UE     ...
    #define USART_CR1_RXNEIE ...

    Rename these if your uart.h uses different names.
*/

static void test_uart_init_success(void) {
    uart_regs_t uart = {0};

    driver_status_t status = uart_init(&uart, 16000000U, 9600U);

    TEST_ASSERT(status == DRIVER_OK);

    TEST_ASSERT((uart.CR1 & USART_CR1_TE) != 0);
    TEST_ASSERT((uart.CR1 & USART_CR1_RE) != 0);
    TEST_ASSERT((uart.CR1 & USART_CR1_UE) != 0);
    TEST_ASSERT((uart.CR1 & USART_CR1_RXNEIE) != 0);

    TEST_ASSERT(uart.BRR != 0);
}

static void test_uart_init_invalid_args(void) {
    uart_regs_t uart = {0};

    TEST_ASSERT(uart_init(NULL, 16000000U, 9600U)
                == DRIVER_ERR_INVALID_ARG);

    TEST_ASSERT(uart_init(&uart, 0U, 9600U)
                == DRIVER_ERR_INVALID_ARG);

    TEST_ASSERT(uart_init(&uart, 16000000U, 0U)
                == DRIVER_ERR_INVALID_ARG);
}

static void test_uart_write_byte_success(void) {
    uart_regs_t uart = {0};

    uart.SR |= USART_SR_TXE;

    driver_status_t status = uart_write_byte(&uart, 0x42, 10U);

    TEST_ASSERT(status == DRIVER_OK);
    TEST_ASSERT((uint8_t)uart.DR == 0x42);
}

static void test_uart_write_byte_timeout(void) {
    uart_regs_t uart = {0};

    /*
        TXE is not set, so the driver should wait until timeout.

        This assumes timeout_ms = 0 causes an immediate timeout.
        If your timeout logic requires millis() to advance, you will need
        a fake millis() in the test build.
    */
    uart.SR &= ~USART_SR_TXE;

    driver_status_t status = uart_write_byte(&uart, 0x42, 0U);

    TEST_ASSERT(status == DRIVER_ERR_TIMEOUT);
}

static void test_uart_write_buffer(void) {
    uart_regs_t uart = {0};

    uint8_t data[] = {
        0x11,
        0x22,
        0x33
    };

    uart.SR |= USART_SR_TXE;

    driver_status_t status = uart_write(&uart,
                                        data,
                                        sizeof(data),
                                        10U);

    TEST_ASSERT(status == DRIVER_OK);

    /*
        Since this fake register only stores the most recent write,
        the final value in DR should be the last byte written.
    */
    TEST_ASSERT((uint8_t)uart.DR == 0x33);
}

static void test_uart_irq_receives_byte(void) {
    uart_regs_t uart = {0};

    /*
        Pretend hardware received one byte.

        RXNE means receive data register not empty.
        DR contains the received byte.
    */
    uart.SR |= USART_SR_RXNE;
    uart.DR = 0x5A;

    uart_irq_handler(&uart);

    uint8_t byte = 0;
    driver_status_t status = uart_read_byte(&byte);

    TEST_ASSERT(status == DRIVER_OK);
    TEST_ASSERT(byte == 0x5A);
}

static void test_uart_read_byte_from_rx_buffer(void) {
    uart_regs_t uart = {0};

    uart.SR |= USART_SR_RXNE;
    uart.DR = 0xA5;

    uart_irq_handler(&uart);

    uint8_t byte = 0;
    driver_status_t status = uart_read_byte(&byte);

    TEST_ASSERT(status == DRIVER_OK);
    TEST_ASSERT(byte == 0xA5);
}

static void test_uart_read_returns_empty_when_no_data(void) {
    uint8_t byte = 0xFF;

    driver_status_t status = uart_read_byte(&byte);

    TEST_ASSERT(status == DRIVER_ERR_EMPTY);
}

static void test_uart_write_buffer_invalid_args(void) {
    uart_regs_t uart = {0};
    uint8_t data[] = {0x11, 0x22};

    TEST_ASSERT(uart_write(NULL, data, sizeof(data), 10U)
                == DRIVER_ERR_INVALID_ARG);

    TEST_ASSERT(uart_write(&uart, NULL, sizeof(data), 10U)
                == DRIVER_ERR_INVALID_ARG);

    TEST_ASSERT(uart_write(&uart, data, 0U, 10U)
                == DRIVER_ERR_INVALID_ARG);
}

int main(void) {
    test_uart_init_success();
    test_uart_init_invalid_args();
    test_uart_write_byte_success();
    test_uart_write_byte_timeout();
    test_uart_write_buffer();
    test_uart_irq_receives_byte();
    test_uart_read_byte_from_rx_buffer();
    test_uart_read_returns_empty_when_no_data();
    test_uart_write_buffer_invalid_args();

    printf("%d/%d tests passed\n", tests_run - tests_failed, tests_run);
    return tests_failed == 0 ? 0 : 1;
}