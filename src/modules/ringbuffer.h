#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>
#include <stdbool.h>

// 环形缓冲区结构体
typedef struct {
    void **buffer;          // 存储数据的缓冲区
    uint32_t capacity;      // 缓冲区容量（必须是2的幂）
    uint32_t mask;          // 容量掩码（capacity - 1，用于快速取模）
    volatile uint32_t read_pos;   // 读位置
    volatile uint32_t write_pos;  // 写位置
} RingBuffer;

// 初始化环形缓冲区
// capacity 必须是2的幂
RingBuffer* ringbuffer_new(uint32_t capacity);

// 释放环形缓冲区
void ringbuffer_free(RingBuffer* rb);

// 检查缓冲区是否为空
bool ringbuffer_is_empty(RingBuffer* rb);

// 检查缓冲区是否已满
bool ringbuffer_is_full(RingBuffer* rb);

// 获取当前可读元素数量
uint32_t ringbuffer_size(RingBuffer* rb);

// 获取缓冲区容量
uint32_t ringbuffer_capacity(RingBuffer* rb);

// 写入一个元素（非阻塞）
// 如果缓冲区满了，覆盖最旧的数据（移动read_pos）
bool ringbuffer_push(RingBuffer* rb, void* item);

// 读取一个元素（非阻塞）
// 如果缓冲区为空，返回 NULL
void* ringbuffer_pop(RingBuffer* rb);

// 批量写入元素
// 返回实际写入的元素数量
uint32_t ringbuffer_push_batch(RingBuffer* rb, void** items, uint32_t count);

// 批量读取元素
// 返回实际读取的元素数量
uint32_t ringbuffer_pop_batch(RingBuffer* rb, void** items, uint32_t count);

// 清空缓冲区
void ringbuffer_clear(RingBuffer* rb);

#endif // RINGBUFFER_H


