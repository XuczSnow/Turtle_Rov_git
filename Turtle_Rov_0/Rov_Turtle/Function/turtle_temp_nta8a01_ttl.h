/**
  ******************************************************************************
  * @file    turtle_temp_nta8a01_ttl.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle ϵ�� ROV ��Դ�¶ȴ�������ȡ����ͷ�ļ�
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#ifndef TURTLE_PRESS_NTA8A01_TTL_H
#define TURTLE_PRESS_NTA8A01_TTL_H

/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

void Temp_Check(void);

int16_t Temp_DataPro(uint8_t *temp_rt);

#endif
