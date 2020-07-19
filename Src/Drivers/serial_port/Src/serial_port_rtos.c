#include "serial_port/common.h"
#include "serial_port/serial_port_rtos.h"

static inline void SerialPort_SetStatusFlags(TX_EVENT_FLAGS_GROUP *status, ULONG flag)
{
    tx_event_flags_set(status, flag, TX_OR);
}
static inline void SerialPort_ClearStatusFlags(TX_EVENT_FLAGS_GROUP *status, ULONG flag)
{
    tx_event_flags_set(status, ~flag, TX_AND);
}
static inline void SerialPort_WaitForStatusFlags(TX_EVENT_FLAGS_GROUP *status, ULONG flag)
{
    ULONG actualFlags;
    tx_event_flags_get(status, flag, TX_AND, &actualFlags, TX_WAIT_FOREVER);
}

static void SerialPort_Send_Async_Buffered_Internal(SerialPort_Rtos *serial_port)
{
    if (!serial_port->Is_Send_Busy_HW(serial_port))
    {
        if (CircleBuffer_IsFull(serial_port->send_buffer))
        {
            SerialPort_ClearStatusFlags(&(serial_port->status), SERIAL_PORT_SEND_AVAILABLE);
        }

        char *data;
        uint32_t length;
        CircleBuffer_PeekToEnd(serial_port->send_buffer, (void **)&data, &length);
        if (length > 0)
        {
            serial_port->Send_Async_HW(serial_port, data, length);
        };
    }
}

int32_t SerialPort_Init(SerialPort_Rtos *serial_port,
                        char *name,
                        CircleBuffer *send_buffer,
                        CircleBuffer *receive_buffer,
                        int32_t (*Init_HW)(SerialPort_Rtos *serial_port, void *init_data), //初始化底层函数s
                        int32_t (*Open_HW)(SerialPort_Rtos *serial_port),
                        int32_t (*Close_HW)(SerialPort_Rtos *serial_port), //打开底层函数
                        int32_t (*Send_Sync_HW)(SerialPort_Rtos *serial_port, char *data, uint32_t length),
                        int32_t (*Send_Async_HW)(SerialPort_Rtos *serial_port, char *data, uint32_t length),
                        uint32_t (*Is_Send_Busy_HW)(SerialPort_Rtos *serial_port),
                        void *init_data)
{
    serial_port->name = name;

    serial_port->send_buffer = send_buffer;
    serial_port->receive_buffer = receive_buffer;

    serial_port->Init_HW = Init_HW;
    serial_port->Open_HW = Open_HW;
    serial_port->Close_HW = Close_HW;
    serial_port->Send_Sync_HW = Send_Sync_HW;
    serial_port->Send_Async_HW = Send_Async_HW;
    serial_port->Is_Send_Busy_HW = Is_Send_Busy_HW;

    tx_mutex_create(&(serial_port->send_busy), name, TX_NO_INHERIT);
    tx_mutex_create(&(serial_port->receive_busy), name, TX_NO_INHERIT);

    tx_event_flags_create(&(serial_port->status), name);
    SerialPort_SetStatusFlags(&(serial_port->status), SERIAL_PORT_SEND_AVAILABLE);
    return 0;
}

void SerialPort_Open(SerialPort_Rtos *serial_port)
{
    serial_port->Open_HW(serial_port);
    SerialPort_SetStatusFlags(&(serial_port->status), SERIAL_PORT_OPEN);
}

void SerialPort_Close(SerialPort_Rtos *serial_port)
{
    serial_port->Close_HW(serial_port);
    SerialPort_ClearStatusFlags(&(serial_port->status), SERIAL_PORT_OPEN);
}

int32_t SerialPort_Send_Sync(SerialPort_Rtos *serial_port, char *data, uint32_t length)
{

    tx_mutex_get(&(serial_port->send_busy), TX_WAIT_FOREVER);

    SerialPort_WaitForStatusFlags(&(serial_port->status), SERIAL_PORT_SEND_AVAILABLE);

    serial_port->Send_Sync_HW(serial_port, data, length);

    tx_mutex_put(&(serial_port->send_busy));
    return length;
}

int32_t SerialPort_Send_Async(SerialPort_Rtos *serial_port, char *data, uint32_t length)
{
    int32_t actualLength = 0;
    tx_mutex_get(&(serial_port->send_busy), TX_WAIT_FOREVER);

    if (serial_port->send_buffer == NULL)
    {
        SerialPort_WaitForStatusFlags(&(serial_port->status), SERIAL_PORT_SEND_AVAILABLE);
        actualLength = serial_port->Send_Async_HW(serial_port, data, length);
    }
    else
    {
        do
        {
            length -= actualLength;
            data += actualLength;
            SerialPort_WaitForStatusFlags(&(serial_port->status), SERIAL_PORT_SEND_AVAILABLE);
            DISABLE_INTERUPT
            actualLength = CircleBuffer_Enqueue(serial_port->send_buffer, data, length, 0);
            ENABLE_INTERUPT

            SerialPort_Send_Async_Buffered_Internal(serial_port);
        } while (actualLength != length);
    }

    tx_mutex_put(&(serial_port->send_busy));
    return actualLength;
}

void SerialPort_Sended_Notify(SerialPort_Rtos *serial_port, uint32_t newTail)
{

    CircleBuffer_SyncTail(serial_port->send_buffer, newTail);
    SerialPort_SetStatusFlags(&(serial_port->status), SERIAL_PORT_SEND_AVAILABLE);
    if (serial_port->send_buffer != NULL)
    {
        SerialPort_Send_Async_Buffered_Internal(serial_port);
    }
}

int32_t SerialPort_Receive_Async(SerialPort_Rtos *serial_port, char *data, uint32_t length)
{
    int32_t actualLength = 0;
    tx_mutex_get(&(serial_port->receive_busy), TX_WAIT_FOREVER);

    SerialPort_WaitForStatusFlags(&(serial_port->status), SERIAL_PORT_RECEIVE_AVAILABLE);

    do
    {
        length -= actualLength;
        data += actualLength;
        if (CircleBuffer_IsEmpty(serial_port->receive_buffer))
        {
            SerialPort_ClearStatusFlags(&(serial_port->status), SERIAL_PORT_RECEIVE_AVAILABLE);
        }
        actualLength = CircleBuffer_Dequeue(serial_port->receive_buffer, data, length);

    } while (actualLength == length);

    tx_mutex_put(&(serial_port->receive_busy));

    return actualLength;
}

void SerialPort_Received_Notify(SerialPort_Rtos *serial_port, uint32_t newHead)
{
    CircleBuffer_SyncHead(serial_port->receive_buffer, newHead);
    SerialPort_SetStatusFlags(&(serial_port->status), SERIAL_PORT_RECEIVE_AVAILABLE);
}