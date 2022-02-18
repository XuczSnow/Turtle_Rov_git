/**
  ******************************************************************************
  * @file    turtle_dbg.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 调试相关函数头文件
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#ifndef TURTLE_DBG_H
#define TURTLE_DBG_H

/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

#define DBGTS_CH1 1u
#define DBGTS_CH2 2u
#define DBGTS_CH3 3u
#define DBGTS_CH4 4u

/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

void Turtle_DBG_TimeStart(uint8_t ch, uint8_t sub);
void Turtle_DBG_TimeStop(uint8_t ch, uint8_t sub);
void JumpToBootloader(void);
void Soft_Reset(void);

#endif