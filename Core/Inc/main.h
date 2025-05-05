/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32l0xx_hal.h"

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Battery_Pin GPIO_PIN_1
#define Battery_GPIO_Port GPIOA
#define SecondaryAlarmTrigger_Pin GPIO_PIN_3
#define SecondaryAlarmTrigger_GPIO_Port GPIOA
#define BoardPowerControl_Pin GPIO_PIN_6
#define BoardPowerControl_GPIO_Port GPIOA
#define Wake_Pin GPIO_PIN_8
#define Wake_GPIO_Port GPIOA
#define AlarmDONE_Pin GPIO_PIN_9
#define AlarmDONE_GPIO_Port GPIOA
#define SecondaryAlarmOverride_Pin GPIO_PIN_10
#define SecondaryAlarmOverride_GPIO_Port GPIOA
#define BackupMonitor_Pin GPIO_PIN_11
#define BackupMonitor_GPIO_Port GPIOA
#define STM32H7_Wake_UP_Pin GPIO_PIN_12
#define STM32H7_Wake_UP_GPIO_Port GPIOA
#define RST_TP_Pin GPIO_PIN_15
#define RST_TP_GPIO_Port GPIOA
#define INT_TP_Pin GPIO_PIN_3
#define INT_TP_GPIO_Port GPIOB
#define INT_TP_EXTI_IRQn EXTI2_3_IRQn
#define BatteryChargeEnable_Pin GPIO_PIN_5
#define BatteryChargeEnable_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
