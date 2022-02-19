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
#include "turtle_pid.h"
#include "turtle_pid_type.h"

#define     AUTO_DEPTH_FLW      1u

extern float u[3];
extern uint8_t de_en;
extern TurtlePid_Type hAuto[4];
extern TurtlePid_Type hDepthAuto[2];

enum TPid_ID
{
  AUTO_DEPTH_ID    = 0,
  AUTO_HEADING_ID  = 1,
  AUTO_PITCH_ID    = 2,
  AUTO_ROLL_ID     = 3,
};

enum TPid_En{
  AUTO_ENABLE   = 0,
  AUTO_DISABLE  = 1,
};

void Turtle_AutoCtrl_Init(void);
void Turtle_AutoCtrl_Refresh(void);
void Turtle_AutoCtrl(TurtlePid_Type *hPid);

void Turtle_AutoCtrl_At(void);
void Turtle_AutoCtrl_EnAt(void);
void Turtle_AutoCtrl_DisAt(void);

void PID_OutOrg(TurtlePid_Type *hPid);

#endif
