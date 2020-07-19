#ifndef __windwolf_serial_port_nortos_H
#define __windwolf_serial_port_nortos_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "stdio.h"
#include "circle_buffer/circle_buffer.h"
#include "tx_api.h"

#define SERIAL_PORT_SEND_AVAILABLE ((ULONG)0b10)
#define SERIAL_PORT_RECEIVE_AVAILABLE ((ULONG)0b100)
#define SERIAL_PORT_OPEN ((ULONG)0b1)

    typedef enum
    {
        IO_BUFFER_STATUS_IDLE,
        IO_BUFFER_STATUS_BUSY,
        IO_BUFFER_STATUS_ERROR,
    } IO_BUFFER_STATUS;

    typedef enum
    {
        SERIAL_PORT_STATUS_READY,
        SERIAL_PORT_STATUS_ONLINE,
        SERIAL_PORT_STATUS_ERROR,
    } SERIAL_PORT_STATUS;

    typedef struct SerialPort_Rtos
    {
        char *name;
        //SERIAL_PORT_STATUS status;
        CircleBuffer *send_buffer;
        CircleBuffer *receive_buffer;
        TX_EVENT_FLAGS_GROUP status;
        TX_MUTEX send_busy;
        TX_MUTEX receive_busy;
        int32_t (*Init_HW)(struct SerialPort_Rtos *serial_port, void *init_data);
        int32_t (*Open_HW)(struct SerialPort_Rtos *serial_port);
        int32_t (*Close_HW)(struct SerialPort_Rtos *serial_port);
        int32_t (*Send_Sync_HW)(struct SerialPort_Rtos *serial_port, char *data, uint32_t length);
        int32_t (*Send_Async_HW)(struct SerialPort_Rtos *serial_port, char *data, uint32_t length);
        uint32_t (*Is_Send_Busy_HW)(struct SerialPort_Rtos *serial_port);
    } SerialPort_Rtos;

    int32_t SerialPort_Init(SerialPort_Rtos *serial_port, //控制块
                            char *name,
                            CircleBuffer *send_buffer,
                            CircleBuffer *receive_buffer,
                            int32_t (*Init_HW)(SerialPort_Rtos *serial_port, void *init_data), //初始化底层函数s
                            int32_t (*Open_HW)(SerialPort_Rtos *serial_port),
                            int32_t (*Close_HW)(SerialPort_Rtos *serial_port), //打开底层函数
                            int32_t (*Send_Sync_HW)(SerialPort_Rtos *serial_port, char *data, uint32_t length),
                            int32_t (*Send_Async_HW)(SerialPort_Rtos *serial_port, char *data, uint32_t length),
                            uint32_t (*Is_Send_Busy_HW)(SerialPort_Rtos *serial_port),
                            void *init_data);

    void SerialPort_Open(SerialPort_Rtos *serial_port);

    void SerialPort_Close(SerialPort_Rtos *serial_port);

    int32_t SerialPort_Send_Sync(SerialPort_Rtos *serial_port, char *data, uint32_t length);

    int32_t SerialPort_Send_Async(SerialPort_Rtos *serial_port, char *data, uint32_t length);

    void SerialPort_Sended_Notify(SerialPort_Rtos *serial_port, uint32_t newTail);

    int32_t SerialPort_Receive_Async(SerialPort_Rtos *serial_port, char *data, uint32_t length);

    void SerialPort_Received_Notify(SerialPort_Rtos *serial_port, uint32_t newHead);

#ifdef __cplusplus
}
#endif

#endif /* __windwolf_serial_port_nortos_H */
