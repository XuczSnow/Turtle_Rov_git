/**
  ******************************************************************************
  * @file    tsch_port.c
  * @author  XuczSnow, OUC/Fab U+
  * @brief   Turtle Scheduler 相关接口文件实现
  * @version Baby 1.0.0
  *
  @verbatim

  @endverbatim
  ******************************************************************************
  @attention

  MIT License

  Copyright (c) 2022 XuczSnow

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  ******************************************************************************
  */

#include "tsch_port.h"

#include <stdio.h>

#include "main.h"
#include "tsch_global.h"

void TSch_UserIdleTask(void){
  //printf("Idle Task Run!\r\n");
  return;
}

void TSch_UserFatal(void){
  printf("Fatal Error!!!");
  return;
}

void SoftExti_WeakInit(void){
  EXTI->IMR |= 1<<MSG_EXTI;
  EXTI->IMR |= 1<<SYN_EXTI;
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 15, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
//  EXTI->SWIER |= 1<<MSG_EXTI;
//  EXTI->SWIER |= 1<<SYN_EXTI;
}

void MsgTask_Weak(uint8_t prio){
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, prio, 0);
  EXTI->SWIER |= 1<<MSG_EXTI;
}

void SynTask_Weak(uint8_t prio){
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, prio, 0);
  EXTI->SWIER |= 1<<SYN_EXTI;
}
