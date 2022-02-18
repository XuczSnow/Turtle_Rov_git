/**
  ******************************************************************************
  * @file    turtle_parameters.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 的参数存储头文件
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
	
#ifndef TURTLE_PARAMETERS_H
#define TURTLE_PARAMETERS_H

#include "turtle_g.h"

#define FLASH_INIT							G_FALSE

#define PID_START_ADD						0x0807FFD8

#define JUMP_BOOTLOADER_ADD			0x0807FFE0

//校验文本 “Turtle Rov by Xucz”
#define VERIFICATION_ADD				0x0807FFF0

//系统固件版本信息
extern const char Hardware_Ver[0x20];
extern const char Firmware_Ver[0x20];
extern const char Compiler_Date[0x20];
extern const char Compiler_Time[0x20];

void Turtle_FlashCheckInit(void);
	
#endif
