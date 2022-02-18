/**
  ******************************************************************************
  * @file    turtle_parameters.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle ϵ�� ROV �Ĳ����洢�ļ�
  *
  ******************************************************************************
  * @attention
  * �ռ����		+0x08000300 - 0x080003FF		�̻���������(256Byte)
	*							-0x80000300 - 0x0800031F		ϵͳӲ���汾
	*							-0x80000320 - 0x0800033F		ϵͳ����汾
	*							-0x80000340 - 0x0800035F		��������
	*							-0x80000360 - 0x0800037F		����ʱ��
	*							-0x80000380 - 0x080003FF		ϵͳԤ��
	*
	*						+0x08070000 - 0x0807FFFF		���޸Ĳ�������(10K)
	*							-0x0807FFF0 - 0x0807FFF7		ϵͳУ����Ϣ
	*							+0x0807FFE0 - 0x0807FFE7		ϵͳ������Ϣ
									+0x0807FFF0
										-bit1										ϵͳ������Bootloader
										
	*							-0x0807FFC0 - 0x0807FFDF		PID�����洢
	*
  *						(����)										ϵͳ��̿ռ�
  ******************************************************************************
  */
	
#ifndef TURTLE_PARAMETER_H
#define TURTLE_PARAMETER_H

#include "turtle_g.h"

//ϵͳ�̼��汾��Ϣ
const char Hardware_Ver[0x20] __attribute__((section(".ARM.__at_0x08000300")))  = "Hardware: 1.0.0";
const char Firmware_Ver[0x20] __attribute__((section(".ARM.__at_0x08000320")))  = "Firmware: 2.0.1";
const char Compiler_Date[0x20] __attribute__((section(".ARM.__at_0x08000340"))) = "Date: "__DATE__;
const char Compiler_Time[0x20] __attribute__((section(".ARM.__at_0x08000360"))) = "Time: "__TIME__;
const char Turtle_Reserve[0x80] __attribute__((section(".ARM.__at_0x08000380")))= {0x00};


//ϵͳУ����Ϣ��ʼ��,
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
