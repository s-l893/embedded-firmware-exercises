#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 64

typedef struct
{
    int head;
    int tail;
    int count;
    uint8_t buffer[BUFFER_SIZE];
} circular_buffer;

void init(circular_buffer *cb)
{
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

bool enqueue(circular_buffer *cb, uint8_t new_data)
{
    if (cb->count == BUFFER_SIZE)
        return false;
    cb->buffer[cb->head] = new_data;
    cb->head = (cb->head + 1) % BUFFER_SIZE;
    cb->count++;
    return true;
}

bool dequeue(circular_buffer *cb, uint8_t *old_data)
{
    if (cb->count == 0)
    {
        return false;
    }
    *old_data = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    cb->count--;
    return true;
}