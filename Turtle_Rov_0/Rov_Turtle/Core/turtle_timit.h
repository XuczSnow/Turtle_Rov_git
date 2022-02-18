/**
  ******************************************************************************
  * @file    turtle_timit.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 定时器中断调度函数头文件
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#ifndef TURTLE_TIMIT_H
#define TURTLE_TIMIT_H

/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

extern uint8_t lstask_cnt;
extern uint8_t lstask_flag;

void Turtle_Timer_LSTask(void);

#endif