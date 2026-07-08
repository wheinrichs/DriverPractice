#include "i2c.h"

static bool wait_until_set(volatile uint32_t *reg, uint32_t mask, uint32_t timeout_ms) {
    uint32_t start = millis();
    while((*reg & mask) == 0U) {
        if(millis() - start >= timeout_ms) {
            return false;
        }
    }
    return true;
}

static bool wait_until_clear(volatile uint32_t *reg, uint32_t mask, uint32_t timeout_ms) {
    uint32_t start = millis();
    while((*reg & mask) != 0U) {
        if(millis() - start >= timeout_ms) {
            return false;
        }
    }
    return true;
}

i2c_status_t i2c_init(i2c_bus_t* i2c, i2c_regs_t* regs) {
    if(i2c == NULL || regs == NULL) {
        return I2C_ERR_INVALID_ARG;
    }
    // This assumes the regs has already enabled the I2C regs in CR
    // This also assumes that main has enabled GPIO clock, configured GPIO as AF, set that AF for I2c
    // Enable the i2c peripheral clock 
    i2c->reg = regs;
    return I2C_OK;
}

i2c_status_t i2c_write_register(i2c_bus_t* i2c, uint32_t reg_to_write_to, uint8_t device, const uint8_t *byte, uint32_t timeout_ms) {
    if(i2c == NULL || byte == NULL) {
        return I2C_ERR_INVALID_ARG;
    }
    // Send start by writing the register in SR 
    // i2c->reg->CR | start_register_mask
    wait_until_set(&i2c->reg->SR, I2C_SR1_TXE, 10);
    uint8_t address_with_write = ((device << 1) | 0);
    i2c->reg->DR = address_with_write;
    wait_until_set(&i2c->reg->SR, I2C_SR1_TXE, 10);
    i2c->reg->DR = byte;
    wait_until_set(&i2c->reg->SR, I2C_SR1_TXE, 10);
    // i2c->reg->CR | stop_register_mask
    return I2C_OK;
}

i2c_status_t i2c_read_reg(i2c_bus_t *bus, uint8_t dev_addr, uint8_t reg_addr, uint8_t*buf, uint16_t len, uint32_t timeout_ms) {
    if(bus == NULL || buf == NULL || len == 0) {
        return I2C_ERR_INVALID_ARG;
    }
    // Send start by writing to the register 
    // bus->reg->CR | start_register_mask
    wait_until_set(&bus->reg->SR, I2C_SR1_TXE, timeout_ms);
    uint8_t address_with_write = ((dev_addr << 1) | 0);
    bus->reg->DR = address_with_write;
    wait_until_set(&bus->reg->SR, I2C_SR1_TXE, timeout_ms);
    bus->reg->DR = reg_addr;
    wait_until_set(&bus->reg->SR, I2C_SR1_TXE, timeout_ms);
    // bus->reg->CR | start_register_mask
    wait_until_set(&bus->reg->SR, I2C_SR1_TXE, timeout_ms);
    uint8_t address_with_read = ((dev_addr << 1) | 1);
    bus->reg->DR = address_with_read;
    for(uint16_t i = 0; i < len; i++) {
        wait_until_set(&bus->reg->SR, I2C_SR1_RXNE, timeout_ms);
        buf[i] = bus->reg->DR;
    }
    // bus->reg->CR | NACK_mask
    wait_until_set(&bus->reg->SR, I2C_SR1_RXNE, timeout_ms);
    // bus->reg->CR | stop_register_mask
    return I2C_OK;
}