#include "gpio.h"
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

static void test_gpio_set_mode_sets_pin_bits(void) {
    gpio_regs_t reg = {0};
    gpio_set_mode(&reg, 0, OUTPUT);
    TEST_ASSERT((reg.MODER & 1U) == OUTPUT);
}


int main(void) {
    test_gpio_set_mode_sets_pin_bits();

    printf("%d/%d tests passed\n", tests_run - tests_failed, tests_run);
    return tests_failed == 0 ? 0 : 1;
}
