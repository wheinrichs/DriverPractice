#include "ring_buffer.h"
#include <stdio.h>

static int tests_run = 0;
static int tests_failed = 0;

#define TEST_ASSERT(cond) do { \
    tests_run++; \
    if (!(cond)) { \
        tests_failed++; \
        printf("FAIL: %s:%d: %s\n", __FILE__, __LINE__, #cond); \
    } \
} while (0)

static void test_empty_start_buffer(void) {
    ring_buffer_t rb;
    rb_init(&rb);
    TEST_ASSERT(true == rb_is_empty(&rb));
}


int main(void) {
    test_empty_start_buffer();

    printf("%d/%d tests passed\n", tests_run - tests_failed, tests_run);
    return tests_failed == 0 ? 0 : 1;
}
