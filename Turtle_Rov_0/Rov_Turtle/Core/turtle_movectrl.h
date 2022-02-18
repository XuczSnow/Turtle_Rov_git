/**
  ******************************************************************************
  * @file    turtle_movectrl.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 运动控制程序
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#ifndef TURTLE_MOVECTRL_H
#define TURTLE_MOVECTRL_H

/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

#define DEPTH_K 0.7			//深度系数控制，全速乘此系数

void Turtle_Prop_UpDown(uint8_t key);
void Turtle_Prop_FwBack(uint8_t key);
void Turtle_Prop_Side(uint8_t key);
void Turtle_Prop_FHeading(uint8_t key);
void Turtle_Prop_BHeading(uint8_t key);
void Turtle_Prop_Heading(uint8_t key);
void Turtle_Prop_Rotation(uint8_t key);
void Turtle_Prop_Pitch(uint8_t key);
void Turtle_Prop_Roll(uint8_t key);

void Turtle_Prop_AtPlus(uint8_t *key);
void Turtle_Prop_AtDrt(uint8_t *key);

void Turtle_Prop_PCLC(void);
void Turtle_Prop_VCLC(void);

#endif
