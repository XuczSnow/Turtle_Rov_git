/**
  ******************************************************************************
  * @file    turtle_host.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle ϵ�� ROV ��λ��ͨ�Ŵ������ͷ�ļ�
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#ifndef TURTLE_HOST_H
#define TURTLE_HOST_H

/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

#define DEPTH_GEAR_EN 0u //ʹ�ܴ����ƽ�����λ

void Turtle_HostMove(void);
void Turtle_HostFb(void);
void Turtle_HostCtrlProp(void);
void Turtle_HostData(uint8_t *data);

#endif
