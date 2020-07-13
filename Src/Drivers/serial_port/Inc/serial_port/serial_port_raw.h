#ifndef __windwolf_serial_port_nortos_H
#define __windwolf_serial_port_nortos_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "stdio.h"
#include "io_buffer/io_buffer.h"

    /* 发送
    * 1. 用户代码产生发送内容, 并调用驱动的发送函数. 内容内存由可能放在线程栈中, 或者动态堆中. 如果是动态堆中, 那么用户代码维护内存;
    * 1.1. 驱动检测到发送忙, 则轮询检测发送Ready;
    * 1.2. 驱动检测到发送忙, 则挂起(RTOS), 直到发送结束;
    * 1.1.1. 驱动直接调用硬件发送;
    * 2.b. 驱动直接调用硬件发送; 或者将发送内容挂入缓冲区, 两种方式: 一种是拷贝入缓冲区; 一种是直接将用户内存挂入缓存链表(怎么解决内存维护问题);
    * 3.b. 驱动
    */

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

    typedef struct
    {
        SERIAL_PORT_STATUS status;
        // IO_BUFFER_STATUS send_status;
        // IO_BUFFER_STATUS receive_status;
        IO_Buffer_TypeDef tx_buffer;
        IO_Buffer_TypeDef rx_buffer;
        TX_EVENT_FLAGS_GROUP busy_flag; 
        TX_SEMAPHORE tx_semaphore;
        TX_SEMAPHORE rx_semaphore;
        int32_t (*Init_HW)(SerialPort_TypeDef *serial_port, void *init_data);
        int32_t (*Open_HW)(SerialPort_TypeDef *serial_port);
        int32_t (*Send_Sync_HW)(SerialPort_TypeDef *serial_port, char *data, uint32_t length);
        int32_t (*Send_Async_HW)(SerialPort_TypeDef *serial_port, char *data, uint32_t length);
        int32_t (*Is_Send_Busy_HW)(SerialPort_TypeDef *serial_port);
        int32_t (*Close_HW)(SerialPort_TypeDef *serial_port);
        int32_t (*Receive_Notify)(SerialPort_TypeDef *serial_port);
    } SerialPort_TypeDef;

    SerialPort_TypeDef *SerialPort_Init(SerialPort_TypeDef *serial_port,                                   //控制块
                                        int32_t (*Init)(SerialPort_TypeDef *serial_port, void *init_data), //初始化底层函数s
                                        int32_t (*Open)(SerialPort_TypeDef *serial_port),
                                        int32_t (*Close)(SerialPort_TypeDef *serial_port), //打开底层函数
                                        int32_t (*Send)(SerialPort_TypeDef *serial_port, IO_Buffer_Block_TypeDef *data),
                                        int32_t (*Is_Send_Busy_HW)(SerialPort_TypeDef *serial_port, IO_Buffer_Block_TypeDef *data),
                                        uint32_t send_buffer_size,
                                        uint32_t receive_buffer_size,
                                        void *init_data);

    void SerialPort_Open(SerialPort_TypeDef *serial_port);

    void SerialPort_Close(SerialPort_TypeDef *serial_port);

    void SerialPort_Send(SerialPort_TypeDef *serial_port, char *data, uint32_t length);

    //void SerialPort_Send_Async(SerialPort_TypeDef *serial_port, char *data, uint32_t length);

    void SerialPort_Receive(SerialPort_TypeDef *serial_port, char *data, uint32_t length, uint32_t timeout);

    //void SerialPort_Receive_Unblock(SerialPort_TypeDef *serial_port, char *data, uint32_t length);












#ifdef __cplusplus
}
#endif

#endif /* __windwolf_serial_port_nortos_H */
