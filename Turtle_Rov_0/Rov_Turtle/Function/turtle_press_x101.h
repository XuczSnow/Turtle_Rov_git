/**
  ******************************************************************************
  * @file    turtle_press_x101.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle ϵ�� ROV ѹ����������ȡ����ͷ�ļ�
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#ifndef TURTLE_PRESS_X101_H
#define TURTLE_PRESS_X101_H

/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

extern float lpt_k;

void Turtle_Press_Init(void);
void Turtle_Press_DataPro(uint8_t *press_rt);

#endif
