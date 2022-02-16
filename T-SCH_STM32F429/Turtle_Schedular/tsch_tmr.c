/**
  ******************************************************************************
  * @file    tsch_tmr.c
  * @author  XuczSnow, OUC/Fab U+
  * @brief   Turtle Scheduler 时间戳处理文件
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

#include "tsch_tmr.h"

#define TASK_MAX_CNT    255u

static TSchTick_Type sTSchTick;         /*定义时间戳处理块，用于时间戳信息保存，禁止外部调用*/

/**
  * @brief  时间戳中断处理函数
  *
  * @note   需要用户在时间基准中断中进行调用
  */
void TSch_TmrIRQHandler(void){
  ++(sTSchTick.tmr);
  if (sTSchTick.tmr == TSchTmr_MAX){
    sTSchTick.tmr = 0;
    ++(sTSchTick.tmr_carry);
  }
}

/*(TODO)是不是可以考虑直接读取定时器的值来获取时间基准 - XuczSnow 2022.02.16*/

/**
  * @brief  任务调度器时间戳初始化
  *
  * @param  tmr_unit 时间戳单位，单位为微秒us
  * @param  tmr_mode 时间基准模式，见TSchTmrMode_Type定义
  * 
  * @retval TSchResState见定义
  */
TSchResState_Type TSch_TmrInit(uint8_t tmr_unit, TSchTmrMode_Type tmr_mode){
  sTSchTick.tmr_unit_us    = tmr_unit;
  sTSchTick.tmr_mode       = tmr_mode;
  sTSchTick.tmr            = 0;

  uint32_t      cnt  = TMR_TIMEOUT;
  TSchTmr_Type  temp = sTSchTick.tmr;
  while (--cnt){
    if (sTSchTick.tmr > temp) return TSCH_OK;
  }
  return TSCH_TIMEOUT;
}

/**
  * @brief  时间戳获取函数
  * 
  * @retval 时间戳
  */
TSchTmr_Type TSch_TmrGet(void){
  return sTSchTick.tmr;
}

/**
  * @brief  时间戳获取进位函数
  * 
  * @retval 时间戳进位数量
  * 
  * @note   时间戳最大值为UINT32_MAX，在长时间运行的情况下，为保证时间准确，
  * 、      可以检查时间戳进位来计算运行时间
  */
TSchTmr_Type TSch_TmrGetCarry(void){
  return sTSchTick.tmr_carry;
}

/**
  * @brief  时间戳数量设置
  *
  * @param  tmr   时间戳设置量，单位见sTSchTick结构体定义
  * 
  * @retval TSchResState见定义
  * 
  * @note   在有外部时间基准，或者用户需要自定义时间戳时，使用此函数，此函数会
  *         修改全局的时间戳
  */
TSchResState_Type TSch_TmrSet(TSchTmr_Type tmr){
  sTSchTick.tmr = tmr;
  return TSCH_OK;
}

static uint8_t __tsch_task_cpu[TASK_MAX_CNT];

/**
  * @brief  任务时间处理函数
  *
  * @param  task    任务处理块，由调度器调用
  * @param  start   任务开始时间
  * @param  last    任务结束时间
  * 
  * @retval TSchResState见定义
  */
TSchResState_Type TSch_TmrTask(TSchTask_Type *task, TSchTmr_Type start, TSchTmr_Type last){
  TSchTmr_Type temp = 0;
  task->tmr_last = start;
  task->tmr_plast = last;
  temp = last - start;
  if (temp > task->tmr_max) task->tmr_max = temp;
  else if (temp < task->tmr_min) task->tmr_min = temp;
  task->tmr_avg = (TSchTmr_Type)(TMR_AVG_K*(float)(task->tmr_avg));

  /*计算CPU利用率并存储*/
  uint64_t task_temp = task->task_cnt*task->tmr_avg;
  uint64_t tmr_temp  = sTSchTick.tmr + sTSchTick.tmr_carry*TSchTmr_MAX;
  __tsch_task_cpu[task->__task_id] = (uint8_t)(100*task_temp/tmr_temp);

  return TSCH_OK;
}

const uint8_t __adt_tim_pr = 10;

/**
  * @brief  任务调度器时间自适应算法实现函数
  *
  * @param  sch     调度器处理块
  * 
  * @retval TSchResState见定义
  */
TSchResState_Type TSch_TmrAdtTime(TScheduler_Type *sch){
  TSchTask_Type *ptask;
  uint8_t temp = 0;
  temp = __tsch_task_cpu[__TaskIdle.__task_id];
  ptask = sch->task_list;
  while (ptask == NULL || ptask->task_next == NULL){
    ptask = ptask->task_next;
  }
  return TSCH_OK;
}