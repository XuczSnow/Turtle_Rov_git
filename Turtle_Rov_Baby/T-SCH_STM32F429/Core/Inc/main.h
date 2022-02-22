/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include <stdio.h>

#include "tsch_global.h"
#include "EventRecorder.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern TScheduler_Type   Tim6_Sch;
extern TSchTask_Type     EH_Tim6Task1;
extern TSchTList_Type    Tim6_List[USR_SCH_LIST_MAX];

extern TScheduler_Type   Tim7_Sch;
extern TSchTask_Type     H_Tim7Task1;
extern TSchTask_Type     H_Tim7Task2;
extern TSchTask_Type     M_Tim7Task3;
extern TSchTask_Type     M_Tim7Task4;
extern TSchTList_Type    Tim7_List[USR_SCH_LIST_MAX];

extern TScheduler_Type   Tim11_Sch;
extern TSchTask_Type     EL_Tim11Task1;

extern TScheduler_Type   Uart_Sch;
extern TSchTask_Type     H_UartTask;

extern TScheduler_Type   Msg_Sch;
extern TSchTask_Type     H_MsgTask1;
extern TSchMsg_Type      High_Msg;
extern TSchTask_Type     L_MsgTask2;
extern TSchMsg_Type      Low_Msg;

extern TScheduler_Type   Syn_Sch;
extern TSchTask_Type     EH_SynTask;

extern uint8_t           uart_test[1];
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
