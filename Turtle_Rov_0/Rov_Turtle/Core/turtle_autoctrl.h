/**
  ******************************************************************************
  * @file    turtle_autoctrl.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 自动控制程序头文件
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#ifndef TURTLE_AUTOCTRL_H
#define TURTLE_AUTOCTRL_H
/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

extern float u[3];
extern uint8_t de_en;

void Turtle_AutoCtrl_Init(void);
void Turtle_AutoCtrl_Refresh(void);
void Turtle_AutoCtrl(AutoCtrl_StateTypeDef *hPid);

void Turtle_AutoCtrl_At(void);
void Turtle_AutoCtrl_EnAt(void);
void Turtle_AutoCtrl_DisAt(void);

void Turtle_AutoCtrl_PID(AutoCtrl_StateTypeDef *hPid);
void Turtle_AutoCtrl_SGPID(AutoCtrl_StateTypeDef *hPid);

void Turtle_SI_Init(void);
void Turtle_SI(SI_StateTypeDef *hSI);
	
#endif
