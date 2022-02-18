/**
  ******************************************************************************
  * @file    tsch_type_def.h
  * @author  XuczSnow, OUC/Fab U+
  * @brief   Turtle Scheduler 数据类型定义头文件，主要包含了各类数据的类型定义
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

#ifndef TSCH_TYPE_DEF_H
#define TSCH_TYPE_DEF_H

#include "tsch_global.h"

/************************************数据类型声明*********************************/

typedef enum      TSchResState    TSchResState_Type;

typedef struct    TSchSem         TSchSem_Type;

typedef uint8_t   TSchMsgEle_Type;
typedef enum      TSchMsgMode     TSchMsgMode_Type;  
typedef struct    TSchMsg         TSchMsg_Type;

typedef enum      TSchMode        TSchMode_Type;
typedef struct    TScheduler      TScheduler_Type;
typedef void      (*TSchExtiPtr)(uint8_t prio);

typedef enum      TSchTaskState   TSchTaskState_Type;
typedef struct    TSchTask        TSchTask_Type;
typedef void      (*TSchTaskPtr)(void *p_arg);

typedef uint32_t  TSchTmr_Type;
typedef enum      TSchTmrMode     TSchTmrMode_Type;
typedef struct    TSchTick        TSchTick_Type;

/************************************数据类型定义*********************************/
/*全局相关定义*/
enum TSchResState{
  TSCH_OK       = 0x00000000,
  TSCH_TIMEOUT  = 0x00000001,         /*操作对象超时*/
  TSCH_INVAILD  = 0x00000002,         /*操作对象无效*/
  TSCH_FULL     = 0x00000003,         /*操作对象满，主要用于串口调度器和消息*/
  TSCH_EMPTY    = 0x00000004,         /*操作对象空，主要用于消息*/
  TSCH_SKIP     = 0x00000005,         /*跳过此次操作，调度器会根据调度情况，跳过某次调度*/
};

/*信号量相关定义*/
struct TSchSem{
  /*信号量相关定义*/
  uint8_t         sem_cnt;
  /*时间戳相关定义*/
  TSchTmr_Type    tmr[USR_SEM_MAX];
};


/*消息相关定义*/
enum TSchMsgMode{
  MSG_NONE      = 0x00040000,
  MSG_STACK     = 0x00040001,
  MSG_QUEUE     = 0x00040002,
};

struct TSchMsg{
  /*消息相关定义*/
  uint16_t          msg_head;
  uint16_t          msg_tail;
  uint16_t          msg_count;
  uint16_t          msg_length;
  TSchMsgMode_Type  msg_mode;
  TSchMsgEle_Type  *msg;
  /*任务唤醒相关定义*/
  TSchTask_Type    *task_wait;
  /*时间戳相关定义*/
  TSchTmr_Type      tmr_last;
};

/*调度器相关定义*/
enum TSchMode{
  UART_SCH      = 0x00010001,         //串口中断调度器
  TIM_SCH       =	0x00010002,         //定时器调度器
  MSG_SCH       = 0x00010003,         //消息唤醒调度器
  SYN_SCH       = 0x00010004,         //同步唤醒调度器
  ADT_TIM_SCH   = 0x00010005,         //自适应时间调度器
  IDLE_SCH      = 0x00010010,         //空闲任务调度器
};

struct TScheduler{
  uint8_t            __tsch_id;       /*调度器内部ID*/
  /*调度器相关定义*/
  TSchMode_Type      tsch_mode;
  uint16_t           tsch_ccnt;       /*调度器运行次数进位值*/
  uint16_t           tsch_cnt;        /*调度器运行次数*/
  /*任务相关定义*/
  uint8_t            task_num;
  TSchTask_Type     *task_list;
  TSchTask_Type     *task_current;
  /*周期任务相关设置，仅在周期调度器中有效*/
  TSchTmr_Type       tsch_period;
  TSchTask_Type    **tsch_list;
};

/*任务相关定义*/
enum TSchTaskState{
  TASK_CREAT    = 0x00020001,           /*任务创建*/
  TASK_RUN      = 0x00020002,           /*任务运行*/
  TASK_READY    = 0x00020003,           /*任务就绪*/
  TASK_WAIT     = 0x00020004,           /*任务等待唤醒*/
  TASK_BLOCK    = 0x00020005,           /*任务阻塞(暂未使用)*/
}; 

struct TSchTask{
  uint8_t             __task_id;        /*任务内部ID*/
  /*任务相关定义*/
  TSchTaskPtr         task_ptr;
  TSchTask_Type      *task_prev;
  TSchTask_Type      *task_next;
  TSchTaskState_Type  task_state;
  uint8_t             task_prio;
  TSchTmr_Type        task_period;
  /*时间统计相关定义*/
  TSchTmr_Type        tmr_start;
  TSchTmr_Type        tmr_last;
  TSchTmr_Type        tmr_plast;
  TSchTmr_Type        tmr_min;
  TSchTmr_Type        tmr_max;
  TSchTmr_Type        tmr_avg;
  uint16_t            task_ccnt;        /*任务运行次数进位值*/
  uint16_t            task_cnt;         /*任务运行计数*/
  /*消息量相关定义*/
  TSchMsg_Type       *msg_wait;
  /*同步唤醒中断服务函数*/
  TSchExtiPtr         syn_funptr;
};

/*时间戳相关定义*/
enum TSchTmrMode{
  TMR_SYSTICK   = 0x00030001,           /*使用系统SYSTICK定时器作为调度器时间基准*/
  TMR_TIM       = 0x00030002,           /*使用定时器作为时间基准*/
  TMR_EXTI      = 0x00030003,           /*使用外部中断时钟源作为基准*/
};

struct TSchTick{
  uint32_t          tmr_unit_us;        /*时钟源单位，以微秒为单位*/
  TSchTmr_Type      tmr_carry;
  TSchTmr_Type      tmr;
  TSchTmrMode_Type  tmr_mode;
};

#endif
/*************************************头文件结束**********************************/
