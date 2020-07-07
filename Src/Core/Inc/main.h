/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_adc.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_i2c.h"
#include "stm32f1xx_ll_iwdg.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void Board_Init();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MOTOR1_FEEDBACK1_Pin LL_GPIO_PIN_0
#define MOTOR1_FEEDBACK1_GPIO_Port GPIOA
#define MOTOR1_FEEDBACK2_Pin LL_GPIO_PIN_1
#define MOTOR1_FEEDBACK2_GPIO_Port GPIOA
#define BATTERY_MONITOR_Pin LL_GPIO_PIN_3
#define BATTERY_MONITOR_GPIO_Port GPIOA
#define MOTOR2_FEEDBACK1_Pin LL_GPIO_PIN_6
#define MOTOR2_FEEDBACK1_GPIO_Port GPIOA
#define MOTOR2_FEEDBACK2_Pin LL_GPIO_PIN_7
#define MOTOR2_FEEDBACK2_GPIO_Port GPIOA
#define MOTOR1_PWM_Pin LL_GPIO_PIN_9
#define MOTOR1_PWM_GPIO_Port GPIOE
#define MOTOR2_PWM_Pin LL_GPIO_PIN_11
#define MOTOR2_PWM_GPIO_Port GPIOE
#define MOTOR1_DIR1_Pin LL_GPIO_PIN_12
#define MOTOR1_DIR1_GPIO_Port GPIOD
#define MOTOR1_DIR2_Pin LL_GPIO_PIN_13
#define MOTOR1_DIR2_GPIO_Port GPIOD
#define MOTOR2_DIR1_Pin LL_GPIO_PIN_14
#define MOTOR2_DIR1_GPIO_Port GPIOD
#define MOTOR2_DIR2_Pin LL_GPIO_PIN_15
#define MOTOR2_DIR2_GPIO_Port GPIOD
#define IMU_I2C_SCL_Pin LL_GPIO_PIN_6
#define IMU_I2C_SCL_GPIO_Port GPIOB
#define IMU_I2C_SDA_Pin LL_GPIO_PIN_7
#define IMU_I2C_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
