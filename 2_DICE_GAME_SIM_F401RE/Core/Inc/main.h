/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define DB_7_Pin GPIO_PIN_13
#define DB_7_GPIO_Port GPIOC
#define DA_1_Pin GPIO_PIN_0
#define DA_1_GPIO_Port GPIOC
#define DA_2_Pin GPIO_PIN_1
#define DA_2_GPIO_Port GPIOC
#define DA_3_Pin GPIO_PIN_2
#define DA_3_GPIO_Port GPIOC
#define DA_4_Pin GPIO_PIN_3
#define DA_4_GPIO_Port GPIOC
#define BTN_ROLL_Pin GPIO_PIN_0
#define BTN_ROLL_GPIO_Port GPIOA
#define BTN_RESET_Pin GPIO_PIN_1
#define BTN_RESET_GPIO_Port GPIOA
#define DA_5_Pin GPIO_PIN_4
#define DA_5_GPIO_Port GPIOC
#define DA_6_Pin GPIO_PIN_5
#define DA_6_GPIO_Port GPIOC
#define LCD_RS_Pin GPIO_PIN_0
#define LCD_RS_GPIO_Port GPIOB
#define LCD_E_Pin GPIO_PIN_1
#define LCD_E_GPIO_Port GPIOB
#define LCD_D4_Pin GPIO_PIN_2
#define LCD_D4_GPIO_Port GPIOB
#define DA_7_Pin GPIO_PIN_6
#define DA_7_GPIO_Port GPIOC
#define DB_1_Pin GPIO_PIN_7
#define DB_1_GPIO_Port GPIOC
#define DB_2_Pin GPIO_PIN_8
#define DB_2_GPIO_Port GPIOC
#define DB_3_Pin GPIO_PIN_9
#define DB_3_GPIO_Port GPIOC
#define BUZZER_Pin GPIO_PIN_8
#define BUZZER_GPIO_Port GPIOA
#define DB_4_Pin GPIO_PIN_10
#define DB_4_GPIO_Port GPIOC
#define DB_5_Pin GPIO_PIN_11
#define DB_5_GPIO_Port GPIOC
#define DB_6_Pin GPIO_PIN_12
#define DB_6_GPIO_Port GPIOC
#define LCD_D5_Pin GPIO_PIN_3
#define LCD_D5_GPIO_Port GPIOB
#define LCD_D6_Pin GPIO_PIN_4
#define LCD_D6_GPIO_Port GPIOB
#define LCD_D7_Pin GPIO_PIN_5
#define LCD_D7_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
