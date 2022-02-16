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
  * @brief  时间自适应调度器周期调整函数
  *
  * @param  sch       目标任务调度器
  * @param  period    需要设置的调度器周期
  * 
  * @note   需要用户在BSP文件中自行实现
  */
__weak void TSch_UserSetPeriod(TScheduler_Type *sch, uint16_t period){

  return;
}

/**
  * @brief  时间自适应调度器周期调整函数
  *
  * @param  sch       目标任务调度器
  * @param  priority  需要设置的调度器优先级
  * 
  * @note   需要用户在BSP文件中自行实现
  */
__weak void TSch_UserSetPriority(TScheduler_Type *sch, uint16_t priority){

  return;
}

TSchResState_Type TSch_SchCreat(TScheduler_Type *sch, TSchMode_Type mode){
  sch->tsch_mode = mode;
  return TSCH_OK;
}

static TSchTmr_Type __Tsch_Gcd(TSchTmr_Type num1, TSchTmr_Type num2){
  TSchTmr_Type temp;
  uint8_t      cnt = 100;
  while (--cnt){
    if (num1%num2 == 0) return num2;
    temp = num2;
    num2 = num1%num2;
    num1 = num2;
  }
  return 0;
}

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

  /*调度器没有任务*/
  if (ptask == NULL && sch->task_num == 0){
    ptask = task;
    sch->task_num = 1;
    TSch_UserSetPriority(sch,task->task_prio);
    if (sch->tsch_mode == TIM_SCH || sch->tsch_mode == ADT_TIM_SCH){
      sch->tsch_period = task->task_period;
    }
    return TSCH_OK;
  }

  /*串口任务及同步任务仅支持添加一个任务*/
  if (sch->tsch_mode != UART_SCH && sch->tsch_mode != SYN_SCH){
    /*有效性检查*/
    if (sch->tsch_mode == MSG_SCH && task->msg_wait == TSCH_MSG_NULL)
      return TSCH_INVAILD;
    else if (sch->tsch_mode == SYN_SCH && task->syn_funptr == NULL)
      return TSCH_INVAILD;

    while (ptask->task_next == NULL) ptask = ptask->task_next;
    ptask->task_next = task;
    ++(sch->task_num);
    if (sch->tsch_mode == TIM_SCH || sch->tsch_mode == ADT_TIM_SCH){
      /*根据最大公约数，设置调度器时间*/
      TSchTmr_Type temp;
      temp = __Tsch_Gcd(task->task_period,sch->tsch_period);
      TSch_UserSetPeriod(sch, temp);
    }
    return TSCH_OK;
  }
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

  if (++(sch->tsch_cnt) == 10000) sch->tsch_cnt = 0;

  switch(sch->tsch_mode){
    case UART_SCH:
    case SYN_SCH:
    case IDLE_SCH:{
      sch->task_current = sch->task_list;
      break;
    }
    case TIM_SCH:
    case ADT_TIM_SCH:{
      uint8_t   run_flag = 0;
      uint32_t  sch_temp = 0;
      uint32_t  task_temp = 0;
      sch->task_current = sch->task_list;
      while (sch->task_current == NULL || sch->task_current->task_next == NULL){
        /*(TODO)为了避免任务冲突，减去了任务id使得任务错开运行，所以建议同一调度器任务连续添加，
          仍存在有些任务恰好被前置任务挤掉的情况，后续寻找更加有利的解决方案 ——XuczSnow 2022.02.16*/
        sch_temp = (sch->tsch_cnt - sch->task_current->__task_id)*sch->tsch_period;
        task_temp = sch->task_current->task_period;
        if (sch_temp%task_temp == 0){
          run_flag = 1;
          break;
        }else{
          sch->task_current = sch->task_current->task_next;
        }
      }
      if (run_flag == 0) return TSCH_SKIP;
    }
    case MSG_SCH:{
      uint8_t run_flag = 0;
      sch->task_current = sch->task_list;
      while (sch->task_current == NULL || sch->task_current->task_next == NULL){
        if (sch->task_current->msg_wait == sch->tsch_waitmsg){
          run_flag = 1;
          break;
        }else{
          sch->task_current = sch->task_current->task_next;
        }
      }
      if (run_flag == 0) return TSCH_SKIP;
      break;
    }
    default:
      return TSCH_INVAILD;
  }

  if (sch->task_current == NULL && sch->task_current->task_ptr == NULL) return TSCH_INVAILD;
  ptask = sch->task_current;
  if (ptask->task_state == TASK_CREAT) ptask->tmr_start = TSch_TmrGet();
  start = TSch_TmrGet();
  ptask->task_state = TASK_RUN;
  if (++(ptask->task_cnt) == 10000) ptask->task_cnt = 0;
  ptask->task_ptr(NULL);
  ptask->task_state = TASK_READY;
  last = TSch_TmrGet();
  res = TSch_TmrTask(ptask, start, last);
  if (res != TSCH_OK) return res;

  if (ptask->task_next != NULL && ptask->task_next->task_prio != 0)
    TSch_UserSetPriority(sch, ptask->task_next->task_prio);
  if (sch->tsch_mode == ADT_TIM_SCH)  TSch_TmrAdtTime(sch);
  return TSCH_OK;
}
