/**
  ******************************************************************************
  * @file    turtle_parameters.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 的参数存储文件
  *
  ******************************************************************************
  * @attention
  * 空间分配		+0x08000300 - 0x080003FF		固化参数保存(256Byte)
	*							-0x80000300 - 0x0800031F		系统硬件版本
	*							-0x80000320 - 0x0800033F		系统软件版本
	*							-0x80000340 - 0x0800035F		编译日期
	*							-0x80000360 - 0x0800037F		编译时间
	*							-0x80000380 - 0x080003FF		系统预留
	*
	*						+0x08070000 - 0x0807FFFF		可修改参数保存(10K)
	*							-0x0807FFF0 - 0x0807FFF7		系统校验信息
	*							+0x0807FFE0 - 0x0807FFE7		系统启动信息
									+0x0807FFF0
										-bit1										系统重启进Bootloader
										
	*							-0x0807FFC0 - 0x0807FFDF		PID参数存储
	*
  *						(其余)										系统编程空间
  ******************************************************************************
  */
	
#ifndef TURTLE_PARAMETER_H
#define TURTLE_PARAMETER_H

#include "turtle_g.h"

//系统固件版本信息
const char Hardware_Ver[0x20] __attribute__((section(".ARM.__at_0x08000300")))  = "Hardware: 1.0.0";
const char Firmware_Ver[0x20] __attribute__((section(".ARM.__at_0x08000320")))  = "Firmware: 2.0.1";
const char Compiler_Date[0x20] __attribute__((section(".ARM.__at_0x08000340"))) = "Date: "__DATE__;
const char Compiler_Time[0x20] __attribute__((section(".ARM.__at_0x08000360"))) = "Time: "__TIME__;
const char Turtle_Reserve[0x80] __attribute__((section(".ARM.__at_0x08000380")))= {0x00};


//系统校验信息初始化,
void Turtle_FlashCheckInit(void)
{
	static uint8_t flash_cnt = 0;
	uint16_t check_info[] = {0x7F70,0xDD70,0x2832,0x4EB7};
	uint16_t check_buf[4];
#if FLASH_INIT == G_TURE
	FLASH_Write(VERIFICATION_ADD,check_info,8u);
#endif
	
	FLASH_Read(VERIFICATION_ADD,check_buf,8u);
	for (uint8_t i=0;i<4;i++)
		while(check_buf[i] != check_info[i])
		{
			HAL_Delay(100);
			flash_cnt ++;
			if (flash_cnt == 128)
				Soft_Reset();
		}
}

#endif
