/**
  ******************************************************************************
  * @file    turtle_prop_485&pwm.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 推进器控制程序头文件
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#ifndef TURTLE_PROP_485PWM_H
#define TURTLE_PROP_485PWM_H

/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

#define CQ_SUM_MAX			0.8		//推进器总量设置
#define PROP_CQ_GMAX		120		//推进器阈值设置


void Turtle_Prop_Init(void);
void Turtle_Prop_Update(void);
void Turtle_Prop_DataPro(uint8_t *prop_rt);
void Turtle_Prop_PwrCuc(void);
void Turtle_Prop_RunCheck(void);
void Turtle_Prop_ErrPro(void);
void Turtle_Prop_GatePro(void);

#endif
