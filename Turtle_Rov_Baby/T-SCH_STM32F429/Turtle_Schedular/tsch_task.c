/**
  ******************************************************************************
  * @file    tsch_task.c
  * @author  XuczSnow, OUC/Fab U+
  * @brief   Turtle Scheduler 任务处理文件，主要包含任务处理函数
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

/*(TODO)后期需要增加任务参数调整的功能 - XuczSnow 2022.02.17*/

#include "tsch_task.h"

static uint8_t __tsch_task_cnt = 0;
/**
  * @brief  任务创建函数
  *
  * @param  sch         调度器处理块，用于将任务添加至调度器
  * @param  task        任务处理块
  * @param  taskptr     任务处理函数
  * @param  period      任务执行周期
  * @param  prio        任务优先级
  * @param  wait_msg    任务调度信号量，无信号量阻塞时，输入MSG_NULL
  * @param  syn_ptr     任务同步唤醒函数
  * 
  * @retval TSchResState见定义
  */
TSchResState_Type TSch_TaskCreat(TScheduler_Type *sch, TSchTask_Type *task, TSchTaskPtr taskptr,\
                                 TSchTmr_Type period, uint8_t prio, TSchMsg_Type *wait_msg,\
                                 TSchExtiPtr syn_ptr){
  TSchResState_Type   res = TSCH_OK;
  
  task->__task_id = (__tsch_task_cnt++);

  if (taskptr == NULL) return TSCH_INVAILD;
  task->task_ptr = taskptr;

  if (prio > 14) return TSCH_INVAILD;
  task->task_prio = prio;

  if (wait_msg != MSG_NULL && syn_ptr == NULL){
    return TSCH_INVAILD;
  }else if (wait_msg != MSG_NULL){
    task->msg_wait = wait_msg;
    task->syn_funptr = syn_ptr;
    task->task_state = TASK_WAIT;
  }else if (syn_ptr != NULL){
    task->syn_funptr = syn_ptr;
    task->task_state = TASK_WAIT;   
  }else{
    task->task_state = TASK_CREAT;
  }

  res = TSch_SchAddTask(sch, task, period);
  return res;
}

//任务唤醒函数
TSchResState_Type TSch_TaskWeak(TSchTask_Type *task){
  if (task->syn_funptr == NULL) return TSCH_INVAILD;
  task->syn_funptr(task->task_prio);
  return TSCH_OK;
}

