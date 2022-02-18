/**
  ******************************************************************************
  * @file    turtle_led_j101.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle ϵ�� ROV ˮ�µƹ������ͷ�ļ�
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#ifndef TURTLE_LED_J101_H
#define TURTLE_LED_J101_H

/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

#define LED_PWM_MAX		1900u
#define LED_PWM_MIN		1u

void Turtle_LED_SetLumin(uint32_t ch, uint16_t pwm);

#endif
