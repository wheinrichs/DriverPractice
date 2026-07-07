#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define RB_SIZE 128

typedef struct {
    uint8_t data[RB_SIZE];
    uint32_t head;
    uint32_t tail;
} ring_buffer_t;

void rb_init(ring_buffer_t* rb);
bool rb_is_empty(const ring_buffer_t *rb);
bool rb_is_full(const ring_buffer_t *rb);
bool rb_push(ring_buffer_t *rb, uint8_t data);
bool rb_pop(ring_buffer_t *rb, uint8_t *out);