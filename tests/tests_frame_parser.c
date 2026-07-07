#include "frame_parser.h"
#include <stdio.h>
#include <stdint.h>

static int tests_run = 0;
static int tests_failed = 0;

#define TEST_ASSERT(cond) do { \
    tests_run++; \
    if (!(cond)) { \
        tests_failed++; \
        printf("FAIL: %s:%d: %s\n", __FILE__, __LINE__, #cond); \
    } \
} while (0)

static void test_parse_valid_frame(void) {
    uint8_t frame[] = {
        SOF,
        3,
        0x10, 0x20, 0x30,
        0x63
    };

    uint8_t payload[FRAME_MAX_PAYLOAD] = {0};
    uint8_t payload_len = 0;

    frame_status_t status = parse_frame(frame,
                                        sizeof(frame),
                                        payload,
                                        &payload_len);

    TEST_ASSERT(status == FRAME_OK);
    TEST_ASSERT(payload_len == 3);
    TEST_ASSERT(payload[0] == 0x10);
    TEST_ASSERT(payload[1] == 0x20);
    TEST_ASSERT(payload[2] == 0x30);
}

static void test_parse_bad_sof(void) {
    uint8_t frame[] = {
        0x55,
        1,
        0x42,
        0x43
    };

    uint8_t payload[FRAME_MAX_PAYLOAD] = {0};
    uint8_t payload_len = 0;

    frame_status_t status = parse_frame(frame,
                                        sizeof(frame),
                                        payload,
                                        &payload_len);

    TEST_ASSERT(status == FRAME_ERR_BAD_SOF);
}

static void test_parse_incomplete_frame(void) {
    uint8_t frame[] = {
        SOF,
        3,
        0x10
    };

    uint8_t payload[FRAME_MAX_PAYLOAD] = {0};
    uint8_t payload_len = 0;

    frame_status_t status = parse_frame(frame,
                                        sizeof(frame),
                                        payload,
                                        &payload_len);

    TEST_ASSERT(status == FRAME_ERR_INCOMPLETE);
}

static void test_parse_payload_too_long(void) {
    uint8_t frame[] = {
        SOF,
        FRAME_MAX_PAYLOAD + 1,
        0x00
    };

    uint8_t payload[FRAME_MAX_PAYLOAD] = {0};
    uint8_t payload_len = 0;

    frame_status_t status = parse_frame(frame,
                                        sizeof(frame),
                                        payload,
                                        &payload_len);

    TEST_ASSERT(status == FRAME_ERR_TOO_LONG);
}

static void test_parse_bad_checksum(void) {
    uint8_t frame[] = {
        SOF,
        2,
        0x10, 0x20,
        0xFF
    };

    uint8_t payload[FRAME_MAX_PAYLOAD] = {0};
    uint8_t payload_len = 0;

    frame_status_t status = parse_frame(frame,
                                        sizeof(frame),
                                        payload,
                                        &payload_len);

    TEST_ASSERT(status == FRAME_ERR_BAD_CHECKSUM);
}

static void test_parse_zero_length_payload(void) {
    uint8_t frame[] = {
        SOF,
        0,
        0
    };

    uint8_t payload[FRAME_MAX_PAYLOAD] = {0xAA};
    uint8_t payload_len = 99;

    frame_status_t status = parse_frame(frame,
                                        sizeof(frame),
                                        payload,
                                        &payload_len);

    TEST_ASSERT(status == FRAME_OK);
    TEST_ASSERT(payload_len == 0);
}

static void test_parse_max_length_payload(void) {
    uint8_t frame[1 + 1 + FRAME_MAX_PAYLOAD + 1] = {0};

    frame[0] = SOF;
    frame[1] = FRAME_MAX_PAYLOAD;

    uint8_t expected_checksum = FRAME_MAX_PAYLOAD;

    for (uint8_t i = 0; i < FRAME_MAX_PAYLOAD; i++) {
        frame[2 + i] = i;
        expected_checksum += i;
    }

    frame[2 + FRAME_MAX_PAYLOAD] = expected_checksum;

    uint8_t payload[FRAME_MAX_PAYLOAD] = {0};
    uint8_t payload_len = 0;

    frame_status_t status = parse_frame(frame,
                                        sizeof(frame),
                                        payload,
                                        &payload_len);

    TEST_ASSERT(status == FRAME_OK);
    TEST_ASSERT(payload_len == FRAME_MAX_PAYLOAD);

    for (uint8_t i = 0; i < FRAME_MAX_PAYLOAD; i++) {
        TEST_ASSERT(payload[i] == i);
    }
}

static void test_parse_invalid_args(void) {
    uint8_t frame[] = {
        SOF,
        0,
        0
    };

    uint8_t payload[FRAME_MAX_PAYLOAD] = {0};
    uint8_t payload_len = 0;

    TEST_ASSERT(parse_frame(NULL, sizeof(frame), payload, &payload_len)
                == FRAME_ERR_INVALID_ARG);

    TEST_ASSERT(parse_frame(frame, sizeof(frame), NULL, &payload_len)
                == FRAME_ERR_INVALID_ARG);

    TEST_ASSERT(parse_frame(frame, sizeof(frame), payload, NULL)
                == FRAME_ERR_INVALID_ARG);
}

int main(void) {
    test_parse_valid_frame();
    test_parse_bad_sof();
    test_parse_incomplete_frame();
    test_parse_payload_too_long();
    test_parse_bad_checksum();
    test_parse_zero_length_payload();
    test_parse_max_length_payload();
    test_parse_invalid_args();

    printf("%d/%d tests passed\n", tests_run - tests_failed, tests_run);
    return tests_failed == 0 ? 0 : 1;
}