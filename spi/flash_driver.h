#include "spi.h"

#define FLASH_CMD_READ_ID 0x9F

bool flash_read_id(spi_device_t* flash, uint8_t  id[3]);