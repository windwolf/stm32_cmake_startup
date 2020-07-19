#include "serial_port/common.h"
#include "serial_port/serial_port_nortos.h"

static void SerialPort_Send_Async_Buffered_Internal(SerialPort_Nortos *serial_port)
{
    if (!serial_port->Is_Send_Busy_HW(serial_port))
    {
        char *data;
        uint32_t length;
        CircleBuffer_PeekToEnd(serial_port->send_buffer, (void **)&data, &length);
        if (length > 0)
        {
            serial_port->Send_Async_HW(serial_port, data, length);
        };
    }
}

int32_t SerialPort_Init(SerialPort_Nortos *serial_port,
                        CircleBuffer *send_buffer,
                        CircleBuffer *receive_buffer,
                        int32_t (*Init_HW)(SerialPort_Nortos *serial_port, void *init_data), //初始化底层函数s
                        int32_t (*Open_HW)(SerialPort_Nortos *serial_port),
                        int32_t (*Close_HW)(SerialPort_Nortos *serial_port), //打开底层函数
                        int32_t (*Send_Sync_HW)(SerialPort_Nortos *serial_port, char *data, uint32_t length),
                        int32_t (*Send_Async_HW)(SerialPort_Nortos *serial_port, char *data, uint32_t length),
                        uint32_t (*Is_Send_Busy_HW)(SerialPort_Nortos *serial_port),
                        void *init_data)
{
    serial_port->status = SERIAL_PORT_STATUS_READY;

    serial_port->send_buffer = send_buffer;
    serial_port->receive_buffer = receive_buffer;

    serial_port->Init_HW = Init_HW;
    serial_port->Open_HW = Open_HW;
    serial_port->Close_HW = Close_HW;
    serial_port->Send_Sync_HW = Send_Sync_HW;
    serial_port->Send_Async_HW = Send_Async_HW;
    serial_port->Is_Send_Busy_HW = Is_Send_Busy_HW;

    Init_HW(serial_port, init_data);

    return 0;
}

void SerialPort_Open(SerialPort_Nortos *serial_port)
{
    serial_port->Open_HW(serial_port);
}

void SerialPort_Close(SerialPort_Nortos *serial_port)
{
    serial_port->Close_HW(serial_port);
}

int32_t SerialPort_Send_Sync(SerialPort_Nortos *serial_port, char *data, uint32_t length)
{
    while (!serial_port->Is_Send_Busy_HW(serial_port))
    {
    }
    serial_port->Send_Sync_HW(serial_port, data, length);
    return length;
}

int32_t SerialPort_Send_Async(SerialPort_Nortos *serial_port, char *data, uint32_t length)
{
    if (serial_port->send_buffer == NULL)
    {
        while (!serial_port->Is_Send_Busy_HW(serial_port))
        {
        }
        return serial_port->Send_Async_HW(serial_port, data, length);
    }
    else
    {
        DISABLE_INTERUPT
        int32_t actualLength = CircleBuffer_Enqueue(serial_port->send_buffer, data, length, 0);
        ENABLE_INTERUPT
        SerialPort_Send_Async_Buffered_Internal(serial_port);
        return actualLength;
    }
}

void SerialPort_Sended_Notify(SerialPort_Nortos *serial_port, uint32_t newTail)
{
    CircleBuffer_SyncTail(serial_port->send_buffer, newTail);
    SerialPort_Send_Async_Buffered_Internal(serial_port);
}

int32_t SerialPort_Receive_Async(SerialPort_Nortos *serial_port, char *data, uint32_t length)
{
    return CircleBuffer_Dequeue(serial_port->receive_buffer, data, length);
}

void SerialPort_Received_Notify(SerialPort_Nortos *serial_port, uint32_t newHead)
{
    CircleBuffer_SyncHead(serial_port->receive_buffer, newHead);
}