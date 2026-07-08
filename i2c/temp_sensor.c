#include "temp_sensor.h"

bool temp_sensor_init(temp_sensor_t* sensor){
    if(sensor == NULL || sensor->bus == NULL) {
        return false;
    }

    uint8_t id = 0U;

    i2c_status_t status = i2c_read_reg(sensor->bus, sensor->address, TEMP_REG_ID, &id, 1, 10U);

    if(status != I2C_OK) {
        return false;
    }

    if(id != TEMP_EXPECTED_ID) {
        return false;
    }

    uint8_t config = 0x00U;

    status = i2c_write_register(sensor->bus, TEMP_REG_CONFIG, sensor->address, &config, 10U);

    if(status != I2C_OK) {
        return false;
    }

    return true;
}

bool temp_sensor_read_celsius(temp_sensor_t* sensor, float* out_c) {
    if(sensor == NULL || out_c == NULL) {
        return false;
    }

    uint8_t temp_out_buffer[2];
    i2c_read_reg(sensor->bus, sensor->address, TEMP_REG_VALUE, temp_out_buffer, 2, 10U);
    *out_c = (float)((uint16_t)(temp_out_buffer[0] << 8) | temp_out_buffer[1]);

    return true;
}