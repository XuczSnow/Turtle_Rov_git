/**
  ******************************************************************************
  * @file    onchip_flash.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   CSDN Ƭ��flash���������ֲ
  *
  ******************************************************************************
  * @attention
  *	�޸�Ƭ��Flash��С
  *
  ******************************************************************************
  */


/**
 * @file  stmflash.h
 *
 * @brief Create by AnKun on 2019/10/10
 *
 */

#ifndef STMFLASH_H__
#define STMFLASH_H__

#include "main.h"

/*======================================= ��ֲ�޸��� =======================================*/

/* FLASH��С��512K */
#define STM32FLASH_SIZE  0x00080000UL

/*======================================= ˽�������� =======================================*/

/* FLASH��ʼ��ַ */
#define STM32FLASH_BASE  FLASH_BASE

/* FLASH������ַ */
#define STM32FLASH_END   (STM32FLASH_BASE | STM32FLASH_SIZE)

/* FLASHҳ��С */
#define STM32FLASH_PAGE_SIZE FLASH_PAGE_SIZE

/* FLASH��ҳ�� */
#define STM32FLASH_PAGE_NUM  (STM32FLASH_SIZE / STM32FLASH_PAGE_SIZE)


/// ������������
void FLASH_Init(void);
uint32_t FLASH_Read(uint32_t Address, void *Buffer, uint32_t Size);
uint32_t FLASH_Write(uint32_t Address, const uint16_t *Buffer, uint32_t NumToWrite);


#endif // !STMFLASH_H__
