/**
  ******************************************************************************
  * @file    tsch_bsp.c
  * @author  XuczSnow, OUC/Fab U+
  * @brief   Turtle Scheduler 接口函数实现
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

#include "main.h"
#include "tim.h"

#define  TIM6_MASK  10u
#define  TIM7_MASK  10u
#define  TIM11_MASK 10u

/*调度器周期设置函数具体实现*/
void TSch_UserSetPeriod(TScheduler_Type *sch, uint16_t period){
  if (sch == &Tim6_Sch)
    __HAL_TIM_SET_AUTORELOAD(&htim6, TIM6_MASK*period);
  else if (sch == &Tim7_Sch)
    __HAL_TIM_SET_AUTORELOAD(&htim7, TIM7_MASK*period);
  else if (sch == &Tim11_Sch)
    __HAL_TIM_SET_AUTORELOAD(&htim11, TIM11_MASK*period);
  else
    return;
}


/*调度器优先级设置函数具体实现*/
void TSch_UserSetPriority(TScheduler_Type *sch, uint16_t priority){
  if (sch == &Tim6_Sch)
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, priority, 0);
  else if (sch == &Tim7_Sch)
    HAL_NVIC_SetPriority(TIM7_IRQn, priority, 0);
  else if (sch == &Tim11_Sch)
    HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, priority, 0);
  else
    return;
}
