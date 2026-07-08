#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define I2C_SR1_TXE     (1U << 7)
#define I2C_SR1_RXNE    (1U << 6)
#define I2C_SR1_BUSY    (1U << 1)
#define I2C_SR1_ACKERR  (1U << 10)

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t DR;
    volatile uint32_t SR;
}i2c_regs_t;

typedef enum {
    I2C_OK=0,
    I2C_ERR_INVALID_ARG,
    I2C_ERR_TIMEOUT,
    I2C_ERR_NACK,
    I2C_ERR_BUSY
}i2c_status_t;

typedef struct {
    i2c_regs_t *reg;
} i2c_bus_t;

i2c_status_t i2c_init(i2c_bus_t* i2c, i2c_regs_t* regs);
i2c_status_t i2c_write_register(i2c_bus_t* i2c, uint32_t reg_to_write_to, uint32_t device, const uint8_t *byte, uint32_t timeout_ms);
i2c_status_t i2c_read_reg(i2c_bus_t *bus, uint8_t dev_addr, uint8_t reg_addr, uint8_t*buf, uint16_t len, uint32_t timeout_ms);