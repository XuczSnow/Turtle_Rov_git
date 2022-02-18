/**
  ******************************************************************************
  * @file    turtle_it.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle ϵ�� ROV �жϹ������ͷ�ļ�
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

//��ʱ������ʹ����̬����
extern uint8_t at_en;
extern uint8_t auto_cycle;
void USER_UART_IdleCpltCallback(UART_HandleTypeDef *huart, UART_IDTypeDef uartNUM);
	
#endif
