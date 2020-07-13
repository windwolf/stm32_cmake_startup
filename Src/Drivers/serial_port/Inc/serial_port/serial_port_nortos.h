#ifndef __windwolf_serial_port_nortos_H
#define __windwolf_serial_port_nortos_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "stdio.h"
#include "circle_queue/circle_buffer.h"

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

    typedef struct SerialPort_Nortos
    {
        SERIAL_PORT_STATUS status;
        CircleBuffer *send_buffer;
        CircleBuffer *receive_buffer;
        int32_t (*Init_HW)(SerialPort_Nortos *serial_port, void *init_data);
        int32_t (*Open_HW)(SerialPort_Nortos *serial_port);
        int32_t (*Close_HW)(SerialPort_Nortos *serial_port);
        int32_t (*Send_Sync_HW)(SerialPort_Nortos *serial_port, char *data, uint32_t length);
        int32_t (*Send_Async_HW)(SerialPort_Nortos *serial_port, char *data, uint32_t length);
        int32_t (*Is_Send_Busy_HW)(SerialPort_Nortos *serial_port);

    } SerialPort_Nortos;

    SerialPort_Nortos *SerialPort_Init(SerialPort_Nortos *serial_port, //控制块
                                       CircleBuffer *send_buffer,
                                       CircleBuffer *receive_buffer,
                                       int32_t (*Init_HW)(SerialPort_Nortos *serial_port, void *init_data), //初始化底层函数s
                                       int32_t (*Open_HW)(SerialPort_Nortos *serial_port),
                                       int32_t (*Close_HW)(SerialPort_Nortos *serial_port), //打开底层函数
                                       int32_t (*Send_Sync_HW)(SerialPort_Nortos *serial_port, char *data, uint32_t length),
                                       int32_t (*Send_Async_HW)(SerialPort_Nortos *serial_port, char *data, uint32_t length),
                                       int32_t (*Is_Send_Busy_HW)(SerialPort_Nortos *serial_port),
                                       void *init_data);

    void SerialPort_Open(SerialPort_Nortos *serial_port);

    void SerialPort_Close(SerialPort_Nortos *serial_port);

    int32_t SerialPort_Send_Sync(SerialPort_Nortos *serial_port, char *data, uint32_t length);

    int32_t SerialPort_Send_Async(SerialPort_Nortos *serial_port, char *data, uint32_t length);

    void SerialPort_Sended_Notify(SerialPort_Nortos *serial_port, uint32_t newTail);

    int32_t SerialPort_Receive_Async(SerialPort_Nortos *serial_port, char *data, uint32_t length);

    void SerialPort_Received_Notify(SerialPort_Nortos *serial_port, uint32_t newHead);

#ifdef __cplusplus
}
#endif

#endif /* __windwolf_serial_port_nortos_H */
