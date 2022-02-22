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
  if (sTSchTick.tmr == TMR_CARRYMAX){
    sTSchTick.tmr = 0;
    ++(sTSchTick.tmr_carry);
  }

#if 0
  /*(TODO)遍历去计算运行时间，感觉不是个好的方法 -XuczSnow 2022.02.22*/
  TScheduler_Type *psch = __tscheduler_list;
  TSchTask_Type *ptask;
  while (psch != NULL){
    ptask = psch->task_list;
    while (ptask != NULL){
      if (ptask->task_state == TASK_RUN){
        ptask->tmr_cnt++;
        return;
      }
      ptask = ptask->task_next;
    }
    psch = psch->tsch_next;
  }
#endif
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
  if (task->task_cnt > G_TMR_AVG_K)
    task->tmr_avg = (TSchTmr_Type)(((G_TMR_AVG_K-1)*task->tmr_avg + temp)/G_TMR_AVG_K);
  else
    task->tmr_avg = temp;

  /*(TODO) CPU利用率计算仍存在问题，整个逻辑仍不太对，任务状态也存在一定的问题 -XuczSnow 2022.02.22*/
  /*计算CPU利用率并存储*/
  uint64_t task_temp;
  uint64_t tmr_temp;
  if (task->tmr_avg == 0)
    task_temp = task->task_cnt/10;
  else 
    task_temp = task->task_cnt*task->tmr_avg;
  tmr_temp  = task->tmr_plast + task->tmr_start[G_TMR_AVG_K-1];
  if (tmr_temp == 0) tmr_temp = 1;
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
  TSchTask_Type *ptask = NULL;
  TSchTmr_Type   temp = 0;
  float   pt = 0;
  float   pr = 0;
  float   pidle = 0;
  float   num = 0;

  pidle = (float)__tsch_task_cpu[__TaskIdle.__task_id];
  pt    = (float)__tsch_task_cpu[ptask->__task_id];
  pr    = (float)TMR_ADT_R;
  num   = (float)sch->task_num;
  ptask = sch->task_list;
  if (ptask == NULL) return TSCH_EMPTY;
  
  if (pt == 0) pt = 1;
  pt = (num*pt + pidle - pr)/num*pt;
  if (pt < 0) return TSCH_INVAILD;
  temp = (TSchTmr_Type)((pt*ptask->task_period)/100);
  temp = (temp/G_TMR_AVG_K + 1)*G_TMR_AVG_K;                    /*调整为TMR_AVG_K的倍数，方式最大公约数计算过小*/

  while (ptask != NULL){
    ptask->task_period = temp;
    ptask = ptask->task_next;
  }
  sch->tsch_period = temp/num;
  return TSCH_OK;
}

TSchTmr_Type TSch_TmrGetUtCpu(TSchTask_Type *task){
  return __tsch_task_cpu[task->__task_id];
}
