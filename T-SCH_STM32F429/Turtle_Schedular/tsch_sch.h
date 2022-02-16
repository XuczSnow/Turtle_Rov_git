/**
  ******************************************************************************
  * @file    tsch_sch.h
  * @author  XuczSnow, OUC/Fab U+
  * @brief   Turtle Scheduler 任务调度器处理头文件
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
#ifndef TSCH_SCH_H
#define TSCH_SCH_H

#include "tsch_global.h"

/**************************************函数声明**********************************/

TSchResState_Type TSch_SchCreat(TScheduler_Type *sch, TSchMode_Type mode);
TSchResState_Type TSch_SchAddTask(TScheduler_Type *sch, TSchTask_Type *task, TSchTmr_Type task_period);
TSchResState_Type TSch_SchRun(TScheduler_Type *sch);

#endif
/*************************************头文件结束**********************************/
