/**
  ******************************************************************************
  * @file    tsch_sch.c
  * @author  XuczSnow, OUC/Fab U+
  * @brief   Turtle Scheduler 任务调度器处理
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

#include "tsch_sch.h"

/**
  * @brief  调度器任务添加
  *
  * @param  sch     调度器处理块
  * @param  task    任务处理块
  * @param  period  调度器执行周期
  * 
  * @retval TSchResState见定义
  */
TSchResState_Type TSch_SchAddTask(TScheduler_Type *sch, TSchTask_Type *task, TSchTmr_Type period){
  TSchTask_Type *ptask = sch->task_list;
  if (ptask == NULL && sch->task_num == 0){
    ptask = task;
    sch->task_num = 1;
    return TSCH_OK;
  }
  /*串口任务仅支持添加一个任务*/
  if (sch->tsch_mode != UART_SCH){
    while (ptask->task_next == NULL) ptask = ptask->task_next;
    ptask->task_next = task;
    ++(sch->task_num);
    return TSCH_OK;
  }
  /*设置调度器时间*/
  TSch_UserSetPeriod(sch, period);
  return TSCH_INVAILD;
}

/**
  * @brief  调度器运行函数，需要在中断中进行调用
  *
  * @param  sch     调度器处理块
  * 
  * @retval TSchResState见定义
  */
TSchResState_Type TSch_SchRun(TScheduler_Type *sch){
  TSchTask_Type      *ptask = NULL;
  TSchTmr_Type        start = 0;
  TSchTmr_Type        last  = 0;
  TSchResState_Type   res   = TSCH_OK;

  uint8_t             run_flag = 0;

  switch(sch->tsch_mode){
    case UART_SCH:{
      sch->task_current = sch->task_list;
      run_flag = 1;
      break;
    }
    case TIM_SCH:
    case ADT_TIM_SCH:{
      if (sch->task_current == NULL || sch->task_current->task_next == NULL)
        sch->task_current = sch->task_list;
      else
        sch->task_current = sch->task_current->task_next;
      run_flag = 1;
      break;
    }
    case EXTI_SCH:{
      sch->task_current = sch->task_list;
      while (sch->task_current == NULL || sch->task_current->task_next == NULL){
        if (sch->task_current->msg_wait == sch->sch_waitmsg){
          run_flag = 1;
          break;
        }else{
          sch->task_current = sch->task_current->task_next;
        }
      }
      break;
    }
    default:
      return TSCH_INVAILD;
  }
  if (run_flag == 0) return TSCH_EMPTY;

  ptask = sch->task_current;
  if (ptask->task_state == TASK_CREAT) ptask->tmr_start = TSch_TmrGet();
  start = TSch_TmrGet();
  ptask->task_state = TASK_RUN;
  ptask->task_ptr(NULL);
  ptask->task_state = TASK_READY;
  last = TSch_TmrGet();
  res = TSch_TmrTask(ptask, start, last);
  if (res != TSCH_OK) return res;

  if (sch->tsch_mode == ADT_TIM_SCH)  TSch_SchAdtTime(sch);
  return TSCH_OK;
}

/**
  * @brief  时间自适应调度器周期调整函数
  *
  * @param  period     需要设置的调度器周期
  * 
  * @note   需要用户在BSP文件中自行实现
  */
__weak void TSch_UserSetPeriod(TScheduler *sch, uint16_t period){

  return;
}

/**
  * @brief  任务调度器时间自适应算法实现函数
  *
  * @param  sch     调度器处理块
  * 
  * @retval TSchResState见定义
  */
TSchResState_Type TSch_SchAdtTime(TScheduler_Type *sch){
  
  return TSCH_OK;
}
