/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SecondaryAlarmTrigger_Pin|BoardPowerControl_Pin|Wake_Pin|AlarmDONE_Pin
                          |SecondaryAlarmOverride_Pin|STM32H7_Wake_UP_Pin|RST_TP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BatteryChargeEnable_GPIO_Port, BatteryChargeEnable_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SecondaryAlarmTrigger_Pin BoardPowerControl_Pin Wake_Pin AlarmDONE_Pin
                           SecondaryAlarmOverride_Pin STM32H7_Wake_UP_Pin RST_TP_Pin */
  GPIO_InitStruct.Pin = SecondaryAlarmTrigger_Pin|BoardPowerControl_Pin|Wake_Pin|AlarmDONE_Pin
                          |SecondaryAlarmOverride_Pin|STM32H7_Wake_UP_Pin|RST_TP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BackupMonitor_Pin */
  GPIO_InitStruct.Pin = BackupMonitor_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BackupMonitor_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : INT_TP_Pin */
  GPIO_InitStruct.Pin = INT_TP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(INT_TP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BatteryChargeEnable_Pin */
  GPIO_InitStruct.Pin = BatteryChargeEnable_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BatteryChargeEnable_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
