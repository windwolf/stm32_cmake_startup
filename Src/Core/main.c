
#include "main.h"

#include "tx_api.h"

#define DEMO_STACK_SIZE 512
#define DEMO_BYTE_POOL_SIZE 1024
#define DEMO_BLOCK_POOL_SIZE 100
#define DEMO_QUEUE_SIZE 100

/* Define the ThreadX object control blocks...  */

TX_THREAD thread_idle;
TX_BYTE_POOL byte_pool_main;
UCHAR memory_area[DEMO_BYTE_POOL_SIZE];

/* Define the counters used in the demo application...  */

ULONG thread_idle_counter;

/* Define thread prototypes.  */

void Idle_threadEntry(ULONG thread_input);
/* USER CODE END Includes */

int main(void)
{
  Board_Init();

  tx_kernel_enter();
}

void tx_application_define(void *first_unused_memory)
{

  CHAR *pointer = TX_NULL;

  /* Create a byte memory pool from which to allocate the thread stacks.  */
  tx_byte_pool_create(&byte_pool_main, "byte pool 0", memory_area, DEMO_BYTE_POOL_SIZE);

  /* Put system definition stuff in here, e.g. thread creates and other assorted
       create information.  */

  /* Allocate the stack for thread 0.  */
  tx_byte_allocate(&byte_pool_main, (VOID **)&pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

  /* Create the main thread.  */
  UINT rtn = tx_thread_create(&thread_idle, "thread 0", Idle_threadEntry, 0,
                              pointer, DEMO_STACK_SIZE,
                              1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);

  UNUSED(rtn);
}

/* Define the test threads.  */

void Idle_threadEntry(ULONG thread_input)
{

  /* This thread simply sits in while-forever-sleep loop.  */
  while (1)
  {
    // 喂狗
    LL_IWDG_ReloadCounter(IWDG);
    /* Increment the thread counter.  */
    thread_idle_counter++;
    LL_GPIO_TogglePin(GPIOE, LL_GPIO_PIN_5);

    // TODO: 低功耗控制
    /* Sleep for 10 ticks.  */
    tx_thread_sleep(100);
  }
}
