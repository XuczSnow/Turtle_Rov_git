/**
  ******************************************************************************
  * @file    tsch_task.c
  * @author  XuczSnow, OUC/Fab U+
  * @brief   Turtle Scheduler 任务示例文件
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
#include "stdio.h"

TSchMsgEle_Type test_shou[1];
TSchTmr_Type    test_tmr;

void Tim6Task1_Func(void *p_arg)
{
  static uint8_t delay;
  test_tmr = TSch_TmrGet();
  if (EH_Tim6Task1.task_cnt%100 == 0)
    delay = test_tmr%10;
  HAL_Delay(delay);
//  printf("Tim6Task1 Run! at %d\r\n", test_tmr);
  return;
}

void Tim7Task1_Func(void *p_arg)
{
  static uint8_t delay;
  test_tmr = TSch_TmrGet();
  if (H_Tim7Task1.task_cnt%100 == 0)
    delay = test_tmr%10; 
  HAL_Delay(delay);
//  printf("Tim7Task1 Run! at %d\r\n", test_tmr);
  return;
}

void Tim7Task2_Func(void *p_arg)
{
  test_tmr = TSch_TmrGet();
  HAL_Delay(5);
//  printf("Tim7Task2 Run! at %d\r\n", test_tmr);
  return;
}

void Tim7Task3_Func(void *p_arg)
{
  test_tmr = TSch_TmrGet();
  HAL_Delay(5);
//  printf("Tim7Task3 Run! at %d\r\n", test_tmr);
  return;
}

void Tim7Task4_Func(void *p_arg)
{
  test_tmr = TSch_TmrGet();
//  printf("Tim7Task4 Run! at %d\r\n", test_tmr);
  return;
}

void UartTask_Func(void *p_arg)
{
  test_tmr = TSch_TmrGet();
  printf("UartTask Run! Com is \"%c\" at %d\r\n", (char)uart_test[0], test_tmr);
  return;
}

void Tim11Task1_Func(void *p_arg)
{
  test_tmr = TSch_TmrGet();
  HAL_Delay(10);
  printf("Tim11Task1 Run!, at %d\r\n", test_tmr);
  test_tmr = TSch_TmrGetUtCpu(&__TaskIdle);
  printf("IdleTask CPU Utilization is %d\r\n", (uint8_t)test_tmr);
  return;
}

void MsgTask1_Func(void *p_arg)
{
  test_tmr = TSch_TmrGet();
  TSch_MsgGet(&High_Msg, test_shou, 1);
  printf("MsgTask1 Run! Msg is \"%c\" at %d\r\n", (char)test_shou[0], test_tmr);
  return;
}

void MsgTask2_Func(void *p_arg)
{
  test_tmr = TSch_TmrGet();
  TSch_MsgGet(&Low_Msg, test_shou, 1);
  printf("MsgTask2 Run! Msg is \"%c\" at %d\r\n", (char)test_shou[0], test_tmr);
  return;
}

void SynTask_Func(void *p_arg)
{
  test_tmr = TSch_TmrGet();
  printf("SynTask1 Run! at %d\r\n", test_tmr);
  return;
}
