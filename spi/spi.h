#include "gpio.h"
#include <stdint.h>

#define SPI_SR_RXNE (1U << 0)
#define SPI_SR_TXE (1U << 1)
#define SPI_SR_BSY (1U << 7)

typedef struct{
    volatile uint32_t CR1;
    volatile uint32_t SR;
    volatile uint32_t DR;
} spi_regs_t;

typedef enum{
    SPI_OK = 0,
    SPI_ERR_INVALID_ARG,
    SPI_ERR_TIMEOUT,
    SPI_ERR_BUSY
}spi_status_t;

typedef struct{
    spi_regs_t *regs;
}spi_bus_t;

spi_status_t spi_init(spi_bus_t* bus, spi_regs_t* regs);
spi_status_t spi_transfer_byte(spi_bus_t *bus, uint8_t data, uint8_t *received_data, uint32_t timeout_ms);
spi_status_t spi_transfer(spi_bus_t *bus, const uint8_t *tx_buf, uint8_t *rx_buf, uint16_t len, uint16_t timeout_ms);

typedef struct {
    spi_bus_t* bus;
    gpio_pin_t* cs;
} spi_device_t;

void spi_select(spi_device_t *dev);
void spi_deselect(spi_device_t *dev);