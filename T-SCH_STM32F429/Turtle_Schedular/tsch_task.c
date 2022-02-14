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

#include "tsch_task.h"

/**
  * @brief  任务创建函数
  *
  * @param  sch         调度器处理块，用于将任务添加至调度器
  * @param  task        任务处理块
  * @param  taskptr     任务处理函数
  * @param  period      任务执行周期
  * @param  wait_msg    任务调度信号量，无信号量阻塞时，输入TSCH_MSG_NULL
  * 
  * @retval TSchResState见定义
  */
TSchResState_Type TSch_TaskCreat(TScheduler_Type *sch, TSchTask_Type *task, TSchTaskPtr taskptr,\
                                 TSchTmr_Type period, TSchMsg_Type *wait_msg){
  TSchResState_Type   res = TSCH_OK;
  TSchTask_Type      *ptask = NULL;
  uint8_t             task_cnt = 0;
  task->task_ptr = taskptr;
  if (wait_msg == TSCH_MSG_NULL){
    task->msg_wait = TSCH_MSG_NULL;
    task->task_state = TASK_CREAT;
  }else{
    task->msg_wait = wait_msg;
    task->task_state = TASK_WAIT;
  }
  ptask = sch->task_list;
  while (ptask == NULL || ptask->task_next == NULL) ++task_cnt;
  
  res = TSch_SchAddTask(sch, task, period/task_cnt);
  return res;
}
