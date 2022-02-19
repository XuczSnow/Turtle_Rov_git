/**
  ******************************************************************************
  * @file    turtle_it.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 中断管理程序头文件
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#ifndef TURTLR_IT_H
#define TURTLR_IT_H

/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

//临时变量，使能姿态控制
extern uint8_t at_en;
extern uint8_t auto_cycle;
void USER_UART_IdleCpltCallback(UART_HandleTypeDef *huart, UART_IDTypeDef uartNUM);
	
#endif
