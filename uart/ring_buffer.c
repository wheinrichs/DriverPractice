#include "ring_buffer.h"

void rb_init(ring_buffer_t* rb) {
    rb->head = 0U;
    rb->tail = 0U;
}

bool rb_is_empty(const ring_buffer_t *rb) {
    if(rb == NULL) {
        return false;
    }
    return rb->head == rb->tail;
}

bool rb_is_full(const ring_buffer_t *rb) {
    if(rb == NULL) {
        return false;
    }
    return ((rb->head + 1) % RB_SIZE) == rb->tail;
}

bool rb_push(ring_buffer_t *rb, uint8_t data) {
    if(rb == NULL || rb_is_full(rb)) {
        return false;
    }
    rb->data[rb->head] = data;
    rb->head = (rb->head + 1) % RB_SIZE;
    return true;
}

bool rb_pop(ring_buffer_t *rb, uint8_t *out) {
    if(rb == NULL || rb_is_empty(rb) || out == NULL) {
        return false;
    }
    *out = rb->data[rb->tail];
    rb->tail = (rb->tail + 1) % RB_SIZE;
    return true;
}