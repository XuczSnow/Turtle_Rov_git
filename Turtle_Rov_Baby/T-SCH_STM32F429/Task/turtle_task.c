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

void Tim6Task1_Func(void *p_arg)
{
  //printf("Tim6Task1 Run!\r\n");
  return;
}

void Tim7Task1_Func(void *p_arg)
{
  //printf("Tim7Task1 Run!\r\n");
  return;
}

void Tim7Task2_Func(void *p_arg)
{
  //printf("Tim7Task2 Run!\r\n");
  return;
}

void Tim7Task3_Func(void *p_arg)
{
  //printf("Tim7Task3 Run!\r\n");
  return;
}

void Tim7Task4_Func(void *p_arg)
{
  //printf("Tim7Task4 Run!\r\n");
  return;
}

void UartTask_Func(void *p_arg)
{
  //printf("UartTask Run!\r\n");
  return;
}

void Tim11Task1_Func(void *p_arg)
{
  //printf("Tim11Task1 Run!\r\n");
  return;
}

void MsgTask1_Func(void *p_arg)
{
  TSch_MsgGet(&High_Msg, test_shou, 1);
  printf("MsgTask1 Run!\r\n");
  return;
}

void MsgTask2_Func(void *p_arg)
{
  printf("MsgTask2 Run!\r\n");
  return;
}

void SynTask_Func(void *p_arg)
{
  printf("SynTask1 Run!\r\n");
  return;
}
