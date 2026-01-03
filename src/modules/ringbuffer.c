#include "ringbuffer.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// 检查是否为2的幂
static bool is_power_of_two(uint32_t n) {
    return n > 0 && (n & (n - 1)) == 0;
}

// 向上取到最近的2的幂
static uint32_t next_power_of_two(uint32_t n) {
    if (n == 0) return 1;
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}

RingBuffer* ringbuffer_new(uint32_t capacity) {
    // 确保容量是2的幂，如果不是则向上取整
    if (!is_power_of_two(capacity)) {
        capacity = next_power_of_two(capacity);
    }
    
    RingBuffer* rb = (RingBuffer*)malloc(sizeof(RingBuffer));
    if (!rb) {
        return NULL;
    }
    
    rb->buffer = (void**)calloc(capacity, sizeof(void*));
    if (!rb->buffer) {
        free(rb);
        return NULL;
    }
    
    rb->capacity = capacity;
    rb->mask = capacity - 1;
    rb->read_pos = 0;
    rb->write_pos = 0;
    
    return rb;
}

void ringbuffer_free(RingBuffer* rb) {
    if (rb) {
        if (rb->buffer) {
            free(rb->buffer);
        }
        free(rb);
    }
}

bool ringbuffer_is_empty(RingBuffer* rb) {
    return rb->read_pos == rb->write_pos;
}

bool ringbuffer_is_full(RingBuffer* rb) {
    return ((rb->write_pos + 1) & rb->mask) == rb->read_pos;
}

uint32_t ringbuffer_size(RingBuffer* rb) {
    uint32_t write = rb->write_pos;
    uint32_t read = rb->read_pos;
    if (write >= read) {
        return write - read;
    } else {
        return rb->capacity - (read - write);
    }
}

uint32_t ringbuffer_capacity(RingBuffer* rb) {
    return rb->capacity;
}

bool ringbuffer_push(RingBuffer* rb, void* item) {
    if (ringbuffer_is_full(rb)) {
        return false;
    }
    
    rb->buffer[rb->write_pos] = item;
    rb->write_pos = (rb->write_pos + 1) & rb->mask;
    return true;
}

void* ringbuffer_pop(RingBuffer* rb) {
    if (ringbuffer_is_empty(rb)) {
        return NULL;
    }
    
    void* item = rb->buffer[rb->read_pos];
    rb->read_pos = (rb->read_pos + 1) & rb->mask;
    return item;
}

uint32_t ringbuffer_push_batch(RingBuffer* rb, void** items, uint32_t count) {
    uint32_t written = 0;
    for (uint32_t i = 0; i < count; i++) {
        if (ringbuffer_push(rb, items[i])) {
            written++;
        } else {
            break;
        }
    }
    return written;
}

uint32_t ringbuffer_pop_batch(RingBuffer* rb, void** items, uint32_t count) {
    uint32_t read = 0;
    for (uint32_t i = 0; i < count; i++) {
        void* item = ringbuffer_pop(rb);
        if (item) {
            items[i] = item;
            read++;
        } else {
            break;
        }
    }
    return read;
}

void ringbuffer_clear(RingBuffer* rb) {
    rb->read_pos = 0;
    rb->write_pos = 0;
    // 清空所有指针引用
    memset(rb->buffer, 0, rb->capacity * sizeof(void*));
}


