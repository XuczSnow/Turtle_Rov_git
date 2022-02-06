#ifndef TSCH_TYPE_DEF
#define TSCH_TYPE_DEF

#include "stdint.h"

/***********************************任务及调度器相关定义********************************/

typedef uint32_t  TSchTmr_Type;

typedef enum      TSchMode        TSchMode_Type;
typedef enum      TSchTaskState   TSchTaskState_Type;
typedef enum      TSchTmrMode     TSchTmrMode_Type;

typedef struct    TSchTask        TSchTask_Type;
typedef struct    TScheduler      TScheduler_Type;

typedef void (*TschTaskPtr)(void *p_arg);

enum TSchMode{
  UART_SCH    = 0x00000000,
  TIM_SCH     =	0x00000001,
  EXTI_SCH    = 0x00000002
};

enum TSchTaskState{
  TASK_RUN    = 0x00010000,
  TASK_READY  = 0x00010001,
  TASK_BLOCK  = 0x00010002
}; 

enum TSchTmrMode{
  TMR_SYSTICK = 0x00020000,
  TMR_TIM     = 0x00020001,
  TMR_EXTI    = 0x00020002
};
struct TschTmr{
  uint32_t          tmr_uint_ns;
  TSchTmr_Type      tmr;
  TSchTmrMode_Type  tmr_mode;
};
struct TSchTask{
  /*任务相关定义*/
  TschTaskPtr     *task_ptr;
  TSchTask_Type   *task_prev;
  TSchTask_Type   *task_next;
  /*时间统计相关定义*/
  TSchTmr_Type    tmr_start;
  TSchTmr_Type    tmr_last;
  TSchTmr_Type    tmr_pstart;
  TSchTmr_Type    tmr_plast;
  TSchTmr_Type    tmr_min;
  TSchTmr_Type    tmr_max;
  TSchTmr_Type    tmr_avg;
};

struct TScheduler{
  /*调度器相关定义*/
  uint8_t           *tsch_name;
  TSchMode_Type      tsch_mode;
  TScheduler_Type   *tsch_prev;
  TScheduler_Type   *tsch_next;
  /*任务相关定义*/
  TSchTask_Type     *task_list;
  TSchTask_Type     *task_current;
};

/**********************************消息队列及消息栈相关定义*******************************/
typedef uint8_t     Element_Type;

typedef TSchMsgStack    TSchMsgStack_Type;
typedef TSchMsgQueue    TSchMsgQueue_Type;

struct TSchMsgStack{
  /*消息相关定义*/
  uint16_t          msg_head;
  uint16_t          msg_tail;
  uint16_t          msg_count;
  uint16_t          msg_length;
  Element_Type      *msg;
  /*时间戳相关定义*/
  TSchTmr_Type      tmr_last;
};

struct TSchMsgQueue{
  /*消息相关定义*/
  uint16_t          msg_head;
  uint16_t          msg_tail;
  uint16_t          msg_count;
  uint16_t          msg_length;
  Element_Type      *msg;
  /*时间戳相关定义*/
  TSchTmr_Type      tmr_last;
};

#endif