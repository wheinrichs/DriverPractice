#include "flash_driver.h"
#include <stdlib.h>

bool flash_read_id(spi_device_t* flash, uint8_t  id[3]) {
    if(flash == NULL) {
        return false;
    }

    uint8_t command[4] = {
        FLASH_CMD_READ_ID,
        0x00,
        0x00,
        0x00
    };

    uint8_t receive[4] = {0};

    spi_select(flash);
    spi_transfer(flash->bus, command, receive, 4, 10U);
    spi_deselect(flash);

    for(uint8_t i = 0; i < 3; i++) {
        id[i] = receive[i+1];
    }

    return true;


}