/**
  ******************************************************************************
  * @file    turtle_host.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 上位机通信处理程序头文件
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

#define DEPTH_GEAR_EN 0u //使能垂向推进器挡位

void Turtle_HostMove(void);
void Turtle_HostFb(void);
void Turtle_HostCtrlProp(void);
void Turtle_HostData(uint8_t *data);

#endif
