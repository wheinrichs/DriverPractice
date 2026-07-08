#include "i2c.h"
#include <stdint.h>

#define TEMP_REG_ID 0x0F
#define TEMP_REG_CONFIG 0x01
#define TEMP_REG_VALUE 0x00
#define TEMP_EXPECTED_ID 0x42

typedef struct{
    i2c_bus_t *bus;
    uint8_t address;
}temp_sensor_t;

bool temp_sensor_init(temp_sensor_t* sensor);
bool temp_sensor_read_celsius(temp_sensor_t* sensor, float* out_c);