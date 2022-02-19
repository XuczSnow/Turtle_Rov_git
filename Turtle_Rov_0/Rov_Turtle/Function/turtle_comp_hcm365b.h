/**
  ******************************************************************************
  * @file    turtle_camp_hcm365b.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 电子罗盘驱动程序头文件
  *
  ******************************************************************************
  * @attention
  *	
  *
  ******************************************************************************
  */

#ifndef TURTLR_CAMP_HCM365B
#define TURTLR_CAMP_HCM365B

/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

void Compass_GetAdd(void);
void Compass_Info(uint8_t *data);

#endif
