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

/**
  * @brief  任务调度器创建
  *
  * @param  sch       目标任务调度器
  * @param  mode      任务调度器模式
  * 
  * @retval TSchResState见定义
  */
TSchResState_Type TSch_SchCreat(TScheduler_Type *sch, TSchMode_Type mode, TSchTList_Type *list){
  static uint8_t __adt_cnt = 0;
  if (mode == ADT_TIM_SCH){
    if (__adt_cnt > 0)
      return TSCH_INVAILD;
    else
      ++ __adt_cnt;
  }
  sch->tsch_mode = mode;
  if (mode == TIM_SCH){
    if (list == NULL) return TSCH_INVAILD;
    sch->task_rdlist = list;
    sch->task_rdlist[USR_SCH_LIST_MAX-1].task_num = 0xA5;
    sch->tsch_pmax   = 0;
    sch->tsch_pmin   = TMR_CARRYMAX;
  }
  return TSCH_OK;
}

/**
  * @brief  周期最大公约数求取
  *
  * @param  num1      第一个数值
  * @param  num2      第二个数值
  * 
  * @retval TSchResState见定义
  */
TSchTmr_Type __Tsch_Gcd(TSchTmr_Type num1, TSchTmr_Type num2){
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

  /*自适应任务调度器任务强制设置为最低优先级*/
  if (sch->tsch_mode == ADT_TIM_SCH) task->task_prio = USR_PRIO_MAX;

  /*调度器没有任务*/
  if (ptask == NULL && sch->task_num == 0){
    sch->task_list = task;
    sch->task_num = 1;
    TSch_UserSetPriority(sch,task->task_prio);
    if (sch->tsch_mode == TIM_SCH || sch->tsch_mode == ADT_TIM_SCH){
      sch->tsch_period = task->task_period;
      sch->tsch_pmax = task->task_period;
      sch->tsch_pmin = task->task_period;
      TSch_UserSetPeriod(sch, sch->tsch_period);
      if (sch->tsch_mode == TIM_SCH){
        /*计算任务就绪列表的长度*/
        TSchTmr_Type temp;
        temp = (sch->tsch_pmax/sch->tsch_period+sch->task_num)*10;
        if (temp > USR_SCH_LIST_MAX) return TSCH_CAERR;             /*若计算无法满足需求，会返回一个计算错误，调整任务周期*/
        else  sch->task_rdlist_len = (uint32_t)temp;
        sch->tsch_cnt = sch->task_rdlist_len+sch->task_dlnum-1;
      }
    }
    return TSCH_OK;
  }

  /*外部消息任务及同步任务仅支持添加一个任务*/
  if (sch->tsch_mode != EXT_SCH && sch->tsch_mode != SYN_SCH){
    /*有效性检查*/
    if (sch->tsch_mode == MSG_SCH){
      if (task->msg_wait == MSG_NULL && task->syn_funptr == NULL)
        return TSCH_INVAILD;
      else if (task->task_prio != sch->task_list->task_prio)
        return TSCH_INVAILD;
    }
    else if (sch->tsch_mode == SYN_SCH && task->syn_funptr == NULL){
      return TSCH_INVAILD;
    }

    if (sch->tsch_mode == TIM_SCH){
      if (task->task_period > sch->tsch_pmax) sch->tsch_pmax = task->task_period;
      if (task->task_period < sch->tsch_pmin) sch->tsch_pmin = task->task_period;
      /*根据最大公约数，设置调度器时间*/
      TSchTmr_Type temp;
      temp = __Tsch_Gcd(task->task_period,sch->tsch_period);
      if (sch->tsch_pmin/temp>TSCH_P_ORDER_MAX) return TSCH_CAERR;
      TSch_UserSetPeriod(sch, temp);
      /*计算任务就绪列表的长度*/
      temp = (sch->tsch_pmax/sch->tsch_period+sch->task_num)*10;
      if (temp > USR_SCH_LIST_MAX) return TSCH_CAERR;             /*若计算无法满足需求，会返回一个计算错误，调整任务周期*/
      else  sch->task_rdlist_len = (uint32_t)temp;
      sch->tsch_cnt = sch->task_rdlist_len+sch->task_dlnum-1;
    }else if (sch->tsch_mode == ADT_TIM_SCH){
      task->task_period = sch->tsch_period*sch->task_num;
      TSch_UserSetPeriod(sch, task->task_period/(sch->task_num+1));
    }
    /*添加任务*/
    while (ptask->task_next != NULL) ptask = ptask->task_next;
    ptask->task_next = task;
    ++(sch->task_num);
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

  //if (++(sch->tsch_cnt) == sch->task_rdlist_len) sch->tsch_cnt = 0, ++(sch->tsch_ccnt);
  ++(sch->tsch_cnt);

  /*任务就绪列表及优先级调整*/
  if (sch->tsch_mode == TIM_SCH){
    if (sch->tsch_cnt == sch->task_rdlist_len+sch->task_dlnum-1 || sch->task_rdlist[sch->tsch_cnt].task == NULL){
      /*清空任务就绪列表*/
      sch->tsch_cnt = 0, ++(sch->tsch_ccnt);
      for (uint8_t i=0;i<sch->task_rdlist_len+sch->task_dlnum;++i){
        sch->task_rdlist[i].task_num = 0;
        sch->task_rdlist[i].task = NULL;
      }
      uint8_t task_num    = 0;
      uint8_t task_cnt    = 0;
      uint8_t period_n    = 0;

      /*计算任务就序列表*/
      sch->task_dlnum = 0;
      for (uint8_t i=0;i<sch->task_rdlist_len+sch->task_dlnum;++i){
        if (sch->task_rdlist[i].task_num == 0xA5)  TSch_FatalError();
        task_cnt = 0;
        ptask = sch->task_list;
        while(ptask != NULL){
          if (task_num < task_cnt) break;
          period_n = ptask->task_period/sch->tsch_period;
          if ((task_num-task_cnt)%period_n == 0){
            if (sch->task_rdlist[i].task == NULL){
              ++task_num;
              sch->task_rdlist[i].task_num = task_num;
              sch->task_rdlist[i].task = ptask;
            }else{
              ++i;
              /*比较优先级*/
              sch->task_rdlist[i].task_num = task_num;
              if (sch->task_rdlist[i-1].task->task_prio <= ptask->task_prio){
                sch->task_rdlist[i].task = ptask;
              }else{
                sch->task_rdlist[i].task = sch->task_rdlist[i-1].task;
                sch->task_rdlist[i-1].task = ptask;
              }
            }
          }
          ptask = ptask->task_next;
          ++ task_cnt;
        }
      }
      TSch_UserSetPriority(sch, sch->task_rdlist[0].task->task_prio);
    }else{
      /*判断是否需要延迟提交*/
      if (sch->task_rdlist[sch->tsch_cnt+1].task_num == sch->task_rdlist[sch->tsch_cnt].task_num){
        TSch_UserSetPeriod(sch, 1);
        sch->task_dlcnt ++;
      }else if(sch->task_rdlist[sch->tsch_cnt-1].task_num == sch->task_rdlist[sch->tsch_cnt].task_num){
        TSch_UserSetPeriod(sch, sch->tsch_period-sch->task_dlcnt);
        sch->task_dlcnt = 0;
      }else{
        TSch_UserSetPeriod(sch,sch->tsch_period);
      }
      /*设置调度器优先级*/
      TSch_UserSetPriority(sch, sch->task_rdlist[sch->tsch_cnt+1].task->task_prio);
    }
  }

  /*时间自适应任务周期调整*/
  if (sch->tsch_mode == ADT_TIM_SCH){
    if (sch->tsch_cnt%TMR_SDT_K == 0){
      TSch_TmrAdtTime(sch);
      TSch_UserSetPeriod(sch, sch->tsch_period);
    }
  }
  
  /*选择任务并运行*/
  switch(sch->tsch_mode){
    case EXT_SCH:
    case SYN_SCH:
    case IDLE_SCH:{
      sch->task_current = sch->task_list;
      break;
    }
    case ADT_TIM_SCH:{
      if (sch->task_current == NULL || sch->task_current->task_next == NULL)
        sch->task_current = sch->task_list;
      else
        sch->task_current = sch->task_current->task_next;
      break;
    }
    case TIM_SCH:{
      sch->task_current = sch->task_rdlist[sch->tsch_cnt].task;
      //printf("%d", sch->tsch_cnt);
      if (sch->task_current == NULL) return TSCH_SKIP;
      break;
    }
    case MSG_SCH:{
      uint8_t run_flag = 0;
      sch->task_current = sch->task_list;
      while (sch->task_current != NULL || sch->task_current->task_next != NULL){
        if (sch->task_current->msg_wait == __tsch_msg_current){
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

  /*任务运行*/
  if (sch->task_current == NULL && sch->task_current->task_ptr == NULL) return TSCH_INVAILD;
  ptask = sch->task_current;
  if (ptask->task_state == TASK_CREAT) ptask->tmr_start = TSch_TmrGet();
  start = TSch_TmrGet();
  ptask->task_state = TASK_RUN;
  if (++(ptask->task_cnt) == USR_TASK_CNT_MAX) ptask->task_cnt = 0, ++(ptask->task_ccnt);
  ptask->task_ptr(NULL);
  ptask->task_state = TASK_READY;
  last = TSch_TmrGet();
  res = TSch_TmrTask(ptask, start, last);
  if (res != TSCH_OK) return res;

  return TSCH_OK;
}
