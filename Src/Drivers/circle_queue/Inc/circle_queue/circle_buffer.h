#ifndef __WINDWOLF_CIRCLE_BUFFER_H_
#define __WINDWOLF_CIRCLE_BUFFER_H_

#include "stdint.h"

typedef enum
{
    CIRCLE_BUFFER_UNIT_SIZE_1 = 0,
    CIRCLE_BUFFER_UNIT_SIZE_2 = 1,
    CIRCLE_BUFFER_UNIT_SIZE_4 = 2,
    CIRCLE_BUFFER_UNIT_SIZE_8 = 3,
} CircleBuffer_UnitSize;

typedef enum
{
    CIRCLE_BUFFER_OPERATION_ENQUEUE,
    CIRCLE_BUFFER_OPERATION_DEQUEUE,
    CIRCLE_BUFFER_OPERATION_SYNC_HEAD,
    CIRCLE_BUFFER_OPERATION_SYNC_TAIL,
    CIRCLE_BUFFER_OPERATION_PEEK_TO_END,
} CircleBuffer_OperationType;

typedef struct
{
    void *data;
    CircleBuffer_UnitSize unitSize;
    uint32_t head; // 指向当前最新的数据
    uint32_t tail;
    uint32_t size;
    uint32_t count;
    union {
        uint8_t allStatus;
        struct
        {
            uint8_t overflowed : 1;
        } statusBits;
    };

    void (*OperationNotify)(CircleBuffer_OperationType type);
    // union {
    //     uint8_t allFlags;
    //     struct
    //     {
    //         uint8_t allowCoverTail : 1;
    //     } flagBits;
    // };

} CircleBuffer;

void CircleBuffer_Create(CircleBuffer *queue, void *dataPtr, CircleBuffer_UnitSize unitSize, uint32_t maxSize);
uint8_t CircleBuffer_IsFull(CircleBuffer *queue);
uint8_t CircleBuffer_IsEmpty(CircleBuffer *queue);
uint8_t CircleBuffer_IsEnoughSpace(CircleBuffer *queue, uint32_t length);
uint8_t CircleBuffer_GetMemoryLength(CircleBuffer *queue);
void *CircleBuffer_GetHeadPointer(CircleBuffer *queue);
void *CircleBuffer_GetTailPointer(CircleBuffer *queue);
uint8_t CircleBuffer_SyncHead(CircleBuffer *queue, uint32_t newHead);
uint8_t CircleBuffer_SyncTail(CircleBuffer *queue, uint32_t newTail);
int32_t CircleBuffer_Enqueue(CircleBuffer *queue, void *valuePtr, uint32_t length, uint8_t allowCoverTail);
int32_t CircleBuffer_Dequeue(CircleBuffer *queue, void *valuePtr, uint32_t length);
int32_t CircleBuffer_PeekToEnd(CircleBuffer *queue, void **valuePtr, uint32_t *length);

void CircleBuffer_RegisterOperationNotify(CircleBuffer *queue, void (*OperationNotify)(CircleBuffer_OperationType type));


#endif //__WINDWOLF_CIRCLE_BUFFER_H_


