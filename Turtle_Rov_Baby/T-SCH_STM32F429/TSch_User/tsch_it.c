/**
  ******************************************************************************
  * @file    tsch_it.c
  * @author  XuczSnow, OUC/Fab U+
  * @brief   Turtle Scheduler 相关中断实现
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
#include "usart.h"

#include "tsch_port.h"

TSchMsgEle_Type test_high[1] = "t";
TSchMsgEle_Type test_low[1] = "l";

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  if(huart == &huart2){
    HAL_UART_Receive_DMA(&huart2, uart_test, 1);
    TSch_SchRun(&Uart_Sch);
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  switch((uint32_t)htim->Instance){
    case (uint32_t)TIM6:{
      TSch_SchRun(&Tim6_Sch);
      break;
    }
    case (uint32_t)TIM7:{
      TSch_SchRun(&Tim7_Sch);
      break;
    }
    case (uint32_t)TIM11:{
      TSch_SchRun(&Tim11_Sch);
      break;
    }
    case (uint32_t)TIM13:{
//      TSch_MsgPub(&High_Msg, test_high, 1);
//      TSch_MsgPub(&Low_Msg, test_low, 1);
//      TSch_TaskWeak(&EH_SynTask);
      break;
    }
  }
}

void EXTI15_10_IRQHandler(void){
  if (EXTI->PR &= 1<<MSG_EXTI)
  {
    EXTI->SWIER &= 0<<MSG_EXTI;
    TSch_SchRun(&Msg_Sch);
  }
  else if (EXTI->PR &= 1<<SYN_EXTI)
  {
    EXTI->SWIER &= 0<<SYN_EXTI;
    TSch_SchRun(&Syn_Sch);    
  }
}
