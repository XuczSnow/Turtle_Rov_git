/**
  ******************************************************************************
  * @file    turtle_cam_hk.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 摄像机管理程序头文件
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#ifndef TURTLE_CAM_HK_H
#define TURTLE_CAM_HK_H

/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

void Turtle_Cam_SetFocus(uint8_t dr);
void OSD_Dsp(void);
void OSD_Clr(void);
void OSD_ReFresh(void);
	
#endif
