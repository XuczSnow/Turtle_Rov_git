#ifndef TSCH_TYPE_DEF
#define TSCH_TYPE_DEF

#include "stdint.h"

typedef uint32_t  TSchTmr;

typedef enum      TSchType        TSchType_Type;
typedef enum      TSchTaskState   TSchTaskState_Type;
typedef struct    TSchTask        TSchTask_Type;
typedef struct    TScheduler      TScheduler_Type;

typedef void (*TschTaskPtr)(void *p_arg);

enum TSchType{
  UART_SCH    = 0x00000000,
  TIM_SCH     =	0x00000001,
  EXTI_SCH    = 0x00000002
};

enum TSchTaskState{
  TASK_RUN    = 0x00010000,
  TASK_READY  = 0x00010001
};  

struct TSchTask{
  TschTaskPtr     *task_ptr;
  TSchTask_Type   *prev_task;
  TSchTask_Type   *next_task;
  //时间统计  
};

struct TScheduler{
  //调度器相关定义
  uint8_t           *spsch_name;
  TSchType_Type     spsch_type;
  TScheduler_Type   *prev_sch;
  TScheduler_Type   *next_sch;
  //任务相关定义
  TSchTask_Type     *task_list;
  TSchTask_Type     *task_current;
};

#endif