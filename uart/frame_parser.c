#include "frame_parser.h"

frame_status_t parse(const uint8_t *buffer, uint16_t len, uint64_t *out, uint8_t* len_out) {
    if(buffer == NULL || out == NULL) {
        return FRAME_ERR_INVALID_ARG;
    }
    if(len < 3) {
        return FRAME_ERR_INCOMPLETE;
    }
    if(buffer[0] != SOF) {
        return FRAME_ERR_BAD_SOF;
    }
    uint8_t frame_length = buffer[1];
    if(frame_length > FRAME_MAX_PAYLOAD) {
        return FRAME_ERR_TOO_LONG;
    } 
    if(frame_length + 3 != len) {
        return FRAME_ERR_INCOMPLETE;
    }

    uint8_t checksum = frame_length;

    for (uint8_t i = 0; i < len; i++) {
        checksum += buffer[2 + i];
    }

    uint8_t received_checksum = buffer[2 + len];

    if (checksum != received_checksum) {
        return FRAME_ERR_BAD_CHECKSUM;
    }

    for (uint8_t i = 0; i < len; i++) {
        out[i] = buffer[2 + i];
    }

    *len_out = len;

    return FRAME_OK;
}