#ifndef __windwolf_iobuffer_H
#define __windwolf_iobuffer_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef IO_BUFFER_BLOCK_SIZE
#define IO_BUFFER_BLOCK_SIZE 32

#endif // !IO_BUFFER_BLOCK_SIZE

#include "tx_api.h"

    typedef enum BUFFER_STATUS
    {
        BUFFER_STATUS_SUCCESS = 0,
        BUFFER_STATUS_EMPTY,

    } BUFFER_STATUS;

    typedef struct
    {
        uint32_t buffer_size;
        IO_Buffer_Block *current_block;
    } IO_Buffer_TypeDef;

    typedef struct IO_Buffer_Block_T
    {
        IO_Buffer_Block *next_buffer;
        char data[IO_BUFFER_BLOCK_SIZE];
        uint32_t buffer_count;
    } IO_Buffer_Block;

    void Buffer_create(IO_Buffer_TypeDef *buffer, void *mem_pointer, uint32_t buffer_size);
    void Buffer_allcate(IO_Buffer_TypeDef *buffer);
    void Buffer_release(IO_Buffer_TypeDef *buffer);
    void Buffer_delete(IO_Buffer_TypeDef *buffer);
    /**
    * 将数据加到buffer队尾
    * */
    BUFFER_STATUS BufferAdd(IO_Buffer_TypeDef *buffer, char *data, uint32_t length);
    /**
     * 从buffer的当前位置获取数据内容
     * */
    BUFFER_STATUS BufferPeek(IO_Buffer_TypeDef *buffer, void **data_pointer, uint32_t *length);
    /**
     * 从buffer的当前位置获取数据内容, 并丢弃
     * */
    BUFFER_STATUS BufferGet(IO_Buffer_TypeDef *buffer, void **data_pointer, uint32_t *length);
    /**
     * 丢弃当前位置的块.
     * */
    BUFFER_STATUS BufferDiscard(IO_Buffer_TypeDef *buffer);

#ifdef __cplusplus
}
#endif

#endif /* __windwolf_iobuffer_H */
