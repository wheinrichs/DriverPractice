#include "spi.h"
#include <stdlib.h>

spi_status_t spi_init(spi_bus_t* bus, spi_regs_t* regs) {
    if(bus == NULL || regs == NULL) {
        return SPI_ERR_INVALID_ARG;
    }

    bus->regs = regs;

    // Would need to do the following:
    // Enable GPIO clock, set GPIO pins to AF, set the AF function for those pins. They would need to be MOSI, MISO, and CLK
    // Also need to configure the CS pin as GPIO
    // enable the clock to SPI via the CRR
    // Set registers for the SPI as follows:
    // Enable SPI bus->regs->CR1 |= SPI_ENABLE
    // Set CPOL bus->regs->CR1 |= CPOL_HIGH 
    // Set CPHA bus->regs->CR1 |= CPHA_HIGH
    // Set Master bus->regs->CR1 |= MASTER_MODE
    // Set baud prescaler - assume we're using 6: bus->regs->CR1 |= (0x6 << BR_SHIFT) 

    return SPI_OK;
}

spi_status_t spi_transfer_byte(spi_bus_t *bus, uint8_t data, uint8_t *received_data, uint32_t timeout_ms){
    if(bus == NULL || received_data == NULL) {
        return SPI_ERR_INVALID_ARG;
    }

    uint32_t start = millis();

    while((bus->regs->SR & SPI_SR_BSY)) {
        if(millis() - start > timeout_ms) {
            return SPI_ERR_BUSY;
        }
    }

    while(!(bus->regs->SR & SPI_SR_TXE)) {
        if(millis() - start > timeout_ms) {
            return SPI_ERR_TIMEOUT;
        }
    }

    bus->regs->DR = data;

    while(!(bus->regs->SR & SPI_SR_RXNE)) {
        if(millis() - start > timeout_ms) {
            return SPI_ERR_TIMEOUT;
        }
    }

    *received_data = bus->regs->DR;
    
    return SPI_OK;
}

spi_status_t spi_transfer(spi_bus_t *bus, const uint8_t *tx_buf, uint8_t *rx_buf, uint16_t len, uint16_t timeout_ms) {
    if(len == 0) {
        return SPI_ERR_INVALID_ARG;
    }

    spi_status_t status;

    for(uint16_t i = 0; i < len; i++) {
        status = spi_transfer_byte(bus, tx_buf[i], rx_buf[i], timeout_ms);
        if(status != SPI_OK) {
            return status;
        }
    }

    return SPI_OK;
}

void spi_select(spi_device_t *dev) {
    gpio_write_pin(dev->cs->regs, dev->cs->pin, false);
}

void spi_deselect(spi_device_t *dev) {
    gpio_write_pin(dev->cs->regs, dev->cs->pin, true);
}