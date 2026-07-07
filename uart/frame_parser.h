#include "uart.h"

#define SOF 0xAA
#define FRAME_MAX_PAYLOAD 64

typedef enum {
    FRAME_OK=0,
    FRAME_ERR_INVALID_ARG,
    FRAME_ERR_INCOMPLETE,
    FRAME_ERR_BAD_SOF,
    FRAME_ERR_TOO_LONG,
    FRAME_ERR_BAD_CHECKSUM
}frame_status_t;

frame_status_t parse(const uint8_t *buffer, uint16_t len, uint64_t *out, uint8_t* len_out);