/**
  ******************************************************************************
  * @file    turtle_yt_j101.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 云台控制程序头文件
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
	
#ifndef TURTLE_YT_J101_H
#define TURTLE_YT_J101_H

/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

#define YT_UP						900u
#define YT_MID					1500u
#define YT_DOWN					2100u

#define YT_ANGLE_MAX		600
#define YT_ANGLE_MIN		-600
#define YT_ANGLE_MMAX		60
#define YT_ANGLE_MMIN		-60

#define YT_ADC_MIN			0x4B0
#define YT_ADC_MAX			0x730
#define YT_ADC_GATE			0x00

#define YT_ADC_CH1			0u
#define YT_ADC_CH2			1u

#define YT_OUTTIME			5000u

void Turtle_YT_InitAngle(void);
void Turtle_YT_MoveC(uint16_t pwm);
int16_t Turtle_YT_GetAngle(void);

#endif
