/**
  ******************************************************************************
  * @file    turtle_timit.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 定时器中断调度函数
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

uint8_t lstask_cnt = 0;
uint8_t lstask_flag= 0;

void Turtle_Timer_LSTask(void)
{
	lstask_flag = 1;
	
	if (lstask_cnt == 7)
		lstask_cnt = 0;
	else if (lstask_cnt%2 == 1)
		lstask_cnt ++;
}