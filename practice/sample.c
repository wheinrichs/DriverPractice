#define FLASH_CMD_READ_ID  0x9F
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    bool (*transfer)(uint8_t tx, uint8_t *rx);
} spi_bus_t;

typedef struct {
    spi_bus_t *bus;
    void (*cs_low)(void);
    void (*cs_high)(void);
} flash_t;

bool flash_read_id(flash_t *flash, uint8_t id[3]) {
    if(flash == NULL || flash->bus == NULL || flash->cs_low == NULL || flash->cs_high == NULL || flash->bus->transfer == NULL || id == NULL) {
        return false;
    }
    uint8_t command[4] = {0};
    uint8_t result[4] = {0};
    command[0] = FLASH_CMD_READ_ID;
    bool status;
    flash->cs_low();
    for(uint8_t i = 0; i < 4; i++) {
        status = flash->bus->transfer(command[i], result[i]);
        if(!status) {
            flash->cs_high();
            return false;
        }
    }
    flash->cs_high();
    return true;
}
Why SPI needs dummy bytes to read data.
SPI needs dummy bytes to read the data because SPI sends and receives on clock pulses. Therefore it needs to send something when reading in order to generate the clock pulses. 

What could go wrong if chip select is not released on failure.
If CS is not released on failure it could prevent subsequent communication because the SPI will be marked as busy and any other lines will be unable to pull it low.  

How you would test this function without real hardware.
You could mock the cs low and cs high functions. You could also mock the transfer function so it writes to a buffer and then it returns a series of bytes. You can then compare that to the result of the function. 