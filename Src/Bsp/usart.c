
#include "usart.h"
#include "dma.h"
#include "serial_port/serial_port_rtos.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_usart.h"
#include "stm32f1xx_hal_dma.h"
#include "stm32f1xx_hal_dma_ex.h"

#define TX_BUFFER_SIZE 64
#define RX_BUFFER_SIZE 64
char tx_buf_data[TX_BUFFER_SIZE];
char rx_buf_data[RX_BUFFER_SIZE];
CircleBuffer tx_buf;
CircleBuffer rx_buf;

void MX_USART1_UART_Init(void)
{
  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**USART1 GPIO Configuration  
    PA9   ------> USART1_TX
    PA10   ------> USART1_RX 
    */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART1 DMA Init */

  /* USART1_TX Init */
  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_4, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_4, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_4, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_4, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_4, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_4, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_4, LL_DMA_MDATAALIGN_BYTE);

  /* USART1_RX Init */
  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_5, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MDATAALIGN_BYTE);

  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(USART1_IRQn);

  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
}

int32_t Init_HW(struct SerialPort_Rtos *serial_port, void *init_data)
{
  MX_DMA_Init();

  MX_USART1_UART_Init();
  LL_USART_EnableIT_IDLE(USART1);

  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_4);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_4);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MODE_CIRCULAR);
  LL_DMA_EnableIT_HT(DMA1, LL_DMA_CHANNEL_5);
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_5);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_5);

  LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t)serial_port->receive_buffer->data);
  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_5, serial_port->receive_buffer->size << serial_port->receive_buffer->unitSize);
  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);
  LL_USART_ClearFlag_IDLE(USART1);
  LL_USART_ClearFlag_ORE(USART1);
  LL_USART_IsEnabledDMAReq_RX(USART1);

  return 0;
}

int32_t Open_HW(struct SerialPort_Rtos *serial_port)
{
  LL_USART_Enable(USART1);
  LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t)serial_port->receive_buffer->data);
  LL_USART_EnableDMAReq_RX(USART1);
  return 0;
}
int32_t Close_HW(struct SerialPort_Rtos *serial_port)
{
  LL_USART_DisableDMAReq_RX(USART1);
  LL_USART_Disable(USART1);
  return 0;
}

int32_t Send_Sync_HW(struct SerialPort_Rtos *serial_port, char *data, uint32_t length)
{

  while (length > 0)
  {
    length--;
    while (LL_USART_IsActiveFlag_TXE(USART1))
    {
    }

    LL_USART_TransmitData8(USART1, *data++);
  }
  return 0;
}

int32_t Send_Async_HW(struct SerialPort_Rtos *serial_port, char *data, uint32_t length)
{
  LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t)data);
  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_4, length);
  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);
  LL_USART_ClearFlag_TC(USART1);
  LL_USART_IsEnabledDMAReq_TX(USART1);
  return 0;
}

uint32_t Is_Send_Busy_HW(struct SerialPort_Rtos *serial_port)
{
  return LL_USART_IsEnabledDMAReq_TX(USART1) || LL_DMA_IsEnabledChannel(DMA1, LL_DMA_CHANNEL_4);
}

void Bsp_Usart1_Init()
{
  CircleBuffer_Create(&tx_buf, tx_buf_data, CIRCLE_BUFFER_UNIT_SIZE_1, TX_BUFFER_SIZE);
  CircleBuffer_Create(&rx_buf, rx_buf_data, CIRCLE_BUFFER_UNIT_SIZE_1, RX_BUFFER_SIZE);
  SerialPort_Init(&bsp_usart1, "usart1", &tx_buf, &rx_buf,
                  Init_HW,
                  Open_HW,
                  Close_HW,
                  Send_Sync_HW,
                  Send_Async_HW,
                  Is_Send_Busy_HW,
                  NULL);
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  if (LL_USART_IsActiveFlag_IDLE(USART1))
  {
    LL_USART_ClearFlag_IDLE(USART1);
  }
  uint32_t count = RX_BUFFER_SIZE - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_5);
  count = count >> CIRCLE_BUFFER_UNIT_SIZE_1;
  SerialPort_Received_Notify(&bsp_usart1, count);
}

/**
  * @brief This function handles DMA1 channel4 global interrupt.
  */
void DMA1_Channel4_IRQHandler(void)
{
  if (LL_DMA_IsActiveFlag_TC4(DMA1))
  {
    LL_DMA_ClearFlag_TC4(DMA1);
    LL_DMA_ClearFlag_GI4(DMA1);
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
  }
  uint32_t newTail = (uint32_t)(bsp_usart1.send_buffer->data) - LL_DMA_GetMemoryAddress(DMA1, LL_DMA_CHANNEL_4) + LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_5);
  SerialPort_Sended_Notify(&bsp_usart1, newTail);
}

/** 
  * @brief This function handles DMA1 channel5 global interrupt.
  */
void DMA1_Channel5_IRQHandler(void)
{
  if (LL_DMA_IsActiveFlag_HT4(DMA1))
  {
    LL_DMA_ClearFlag_HT4(DMA1);
  }
  if (LL_DMA_IsActiveFlag_TC4(DMA1))
  {
    LL_DMA_ClearFlag_TC4(DMA1);
  }
  uint32_t count = RX_BUFFER_SIZE - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_5);
  count = count >> CIRCLE_BUFFER_UNIT_SIZE_1;
  SerialPort_Received_Notify(&bsp_usart1, count);
}