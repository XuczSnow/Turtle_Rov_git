/**
  ******************************************************************************
  * @file    turtle_parameters.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle ϵ�� ROV �Ĳ����洢ͷ�ļ�
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

//У���ı� ��Turtle Rov by Xucz��
#define VERIFICATION_ADD				0x0807FFF0

//ϵͳ�̼��汾��Ϣ
extern const char Hardware_Ver[0x20];
extern const char Firmware_Ver[0x20];
extern const char Compiler_Date[0x20];
extern const char Compiler_Time[0x20];

void Turtle_FlashCheckInit(void);
	
#endif
