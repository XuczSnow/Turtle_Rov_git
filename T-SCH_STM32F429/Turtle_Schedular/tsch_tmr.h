/**
  ******************************************************************************
  * @file    tsch_tmr.h
  * @author  XuczSnow, OUC/Fab U+
  * @brief   Turtle Scheduler 时间戳处理文件头文件
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
#ifndef TSCH_TMR_H
#define TSCH_TMR_H

#include "tsch_global.h"

/*************************************宏定义声明**********************************/

#define TMR_AVG_K           20u
#define TMR_TIMEOUT         10000u
#define TSchTmr_MAX         UINT32_MAX

/**************************************函数声明**********************************/

void TSch_TmrIRQHandler(void);

TSchResState_Type TSch_TmrInit(uint8_t tmr_uint, TSchTmrMode_Type tmr_mode);
TSchTmr_Type TSch_TmrGet(void);
TSchResState_Type TSch_TmrSet(TSchTmr_Type tmr);
TSchResState_Type TSch_TmrTask(TSchTask_Type *ptask, TSchTmr_Type start, TSchTmr_Type last);
TSchResState_Type TSch_TmrAdtTime(TScheduler_Type *sch);

#endif
/*************************************头文件结束**********************************/
