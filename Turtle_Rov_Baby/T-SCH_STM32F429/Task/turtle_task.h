/**
  ******************************************************************************
  * @file    tsch_task.h
  * @author  XuczSnow, OUC/Fab U+
  * @brief   Turtle Scheduler 任务示例头文件
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

#ifndef TURTLE_TASK_H
#define TURTLE_TASK_H

void Tim6Task1_Func(void *p_arg);
void Tim7Task1_Func(void *p_arg);
void Tim7Task2_Func(void *p_arg);
void Tim7Task3_Func(void *p_arg);
void Tim7Task4_Func(void *p_arg);
void Tim11Task1_Func(void *p_arg);
void UartTask_Func(void *p_arg);
void MsgTask1_Func(void *p_arg);
void MsgTask2_Func(void *p_arg);
void SynTask_Func(void *p_arg);

#endif