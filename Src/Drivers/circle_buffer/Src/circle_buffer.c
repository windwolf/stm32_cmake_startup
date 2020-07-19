#include "stdint.h"
#include "circle_buffer/circle_buffer.h"
#include "stdlib.h"
//#include "portable.h"

#define CONVERT_TO_UINT8_POINTER(a) ((uint8_t *)((void *)(a)))
#define CONVERT_TO_VOID_POINTER(a) ((void *)(a))
#define POINTER_ADD(a, b) (((uint8_t *)((void *)(a))) + ((int32_t)(b)))

void CircleBuffer_Create(CircleBuffer *queue, void *dataPtr, CircleBuffer_UnitSize unitSize, uint32_t maxSize)
{
    queue->size = maxSize;
    queue->unitSize = unitSize;
    queue->statusBits.overflowed = 0;
    queue->data = dataPtr;
    queue->head = 0;
    queue->count = 0;
    queue->tail = 0;
}

void CircleBuffer_RegisterOperationNotify(CircleBuffer *queue, void (*OperationNotify)(CircleBuffer_OperationType type))
{
    queue->OperationNotify = OperationNotify;
}

inline uint8_t CircleBuffer_IsFull(CircleBuffer *queue)
{
    return queue->count == queue->size;
}

inline uint8_t CircleBuffer_IsEmpty(CircleBuffer *queue)
{
    return queue->count == 0;
}

inline uint8_t CircleBuffer_IsEnoughSpace(CircleBuffer *queue, uint32_t length)
{
    return (queue->size - queue->count) >= length;
}

inline uint8_t CircleBuffer_GetMemoryLength(CircleBuffer *queue)
{
    return queue->count << queue->unitSize;
}

inline void *CircleBuffer_GetHeadPointer(CircleBuffer *queue)
{
    return (void *)POINTER_ADD(queue->data, (queue->head) << (queue->unitSize));
}

inline void *CircleBuffer_GetTailPointer(CircleBuffer *queue)
{
    return (void *)POINTER_ADD(queue->data, (queue->tail) << (queue->unitSize));
}

uint8_t CircleBuffer_SyncHead(CircleBuffer *queue, uint32_t newHead)
{
    uint32_t tail = queue->tail;
    uint32_t size = queue->size;
    uint32_t count = queue->count;
    uint32_t head = queue->head;
    int32_t headDiff = newHead - head;
    uint32_t length = headDiff >= 0 ? headDiff : headDiff + size;
    uint8_t isEnough = (size - count) >= length;

    count += length;
    head = newHead;

    if (!isEnough)
    {
        queue->statusBits.overflowed = 1;
        count = size;
        queue->tail = newHead;
    }
    queue->count = count;
    queue->head = newHead;

    if (queue->OperationNotify != NULL)
    {
        queue->OperationNotify(CIRCLE_BUFFER_OPERATION_SYNC_HEAD);
    }
    return 0;
}

uint8_t CircleBuffer_SyncTail(CircleBuffer *queue, uint32_t newTail)
{
    uint32_t tail = queue->tail;
    uint32_t count = queue->count;
    int32_t tailDiff = newTail - tail;
    uint32_t length = tailDiff >= 0 ? tailDiff : tailDiff + queue->size;

    if (count < length)
    {
        return -1;
    }

    queue->count -= length;
    queue->tail = newTail;

    if (queue->OperationNotify != NULL)
    {
        queue->OperationNotify(CIRCLE_BUFFER_OPERATION_SYNC_TAIL);
    }
    return 0;
}

int32_t CircleBuffer_Enqueue(CircleBuffer *queue, void *valuePtr, uint32_t length, uint8_t allowCoverTail)
{
    uint8_t unitSize = queue->unitSize;
    uint32_t head = queue->head;
    uint32_t tail = queue->tail;
    uint32_t size = queue->size;
    uint32_t count = queue->count;
    uint32_t countFormHeadToBufferEnd = size - head;
    uint32_t rtnLength = length;

    uint8_t isEnough = (size - count) >= length;
    if (!isEnough)
    {
        if (allowCoverTail)
        {
            if (length > size)
            {
                valuePtr = POINTER_ADD(valuePtr, (length - size) << unitSize);
                length = size;
            }
            queue->statusBits.overflowed = 1;
        }
        else
        {
            length = (size - count);
            rtnLength = length;
        }
    }

    if (countFormHeadToBufferEnd >= length)
    {
        memcpy(POINTER_ADD(queue->data, head << unitSize), valuePtr, length << unitSize);
        head += length;
        head = head == size ? 0 : head;
        count += length;
    }
    else
    {

        memcpy(POINTER_ADD(queue->data, head << unitSize), valuePtr, countFormHeadToBufferEnd << unitSize);
        memcpy(queue->data, POINTER_ADD(valuePtr, countFormHeadToBufferEnd << unitSize), (length - countFormHeadToBufferEnd) << unitSize);

        head = length - countFormHeadToBufferEnd;
        count += length;
    }

    if (!isEnough)
    {
        count = size;
        tail = head;
        queue->tail = tail;
    }

    queue->count = count;
    queue->head = head;

    if (queue->OperationNotify != NULL)
    {
        queue->OperationNotify(CIRCLE_BUFFER_OPERATION_ENQUEUE);
    }
    return rtnLength;
}

int32_t CircleBuffer_Dequeue(CircleBuffer *queue, void *valuePtr, uint32_t length)
{
    uint8_t unitSize = queue->unitSize;
    uint32_t tail = queue->tail;
    uint32_t size = queue->size;
    uint32_t count = queue->count;
    uint32_t countFormTailToBufferEnd = size - tail;
    if (count < length)
    {
        length = count;
    }

    if (countFormTailToBufferEnd >= length)
    {
        memcpy(valuePtr, POINTER_ADD(queue->data, tail << unitSize), length << unitSize);
        tail += length;
        tail = tail == size ? 0 : tail;
        count -= length;
    }
    else
    {
        memcpy(valuePtr, POINTER_ADD(queue->data, tail << unitSize), countFormTailToBufferEnd << unitSize);
        memcpy(POINTER_ADD(valuePtr, countFormTailToBufferEnd << unitSize), queue->data, (length - countFormTailToBufferEnd) << unitSize);

        tail = length - countFormTailToBufferEnd;
        count += length;
    }

    queue->count = count;
    queue->tail = tail;
    queue->statusBits.overflowed = 0;

    if (queue->OperationNotify != NULL)
    {
        queue->OperationNotify(CIRCLE_BUFFER_OPERATION_DEQUEUE);
    }
    return length;
}

uint8_t CircleBuffer_PeekToEnd(CircleBuffer *queue, void **valuePtr, uint32_t *length)
{
    uint8_t unitSize = queue->unitSize;
    uint32_t tail = queue->tail;
    uint32_t size = queue->size;
    uint32_t count = queue->count;
    uint32_t countFormTailToBufferEnd = size - tail;

    *valuePtr = POINTER_ADD(queue->data, queue->tail << unitSize);
    *length = count < countFormTailToBufferEnd ? count : countFormTailToBufferEnd;

    if (queue->OperationNotify != NULL)
    {
        queue->OperationNotify(CIRCLE_BUFFER_OPERATION_PEEK_TO_END);
    }
    return 0;
}
