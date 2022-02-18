/**
  ******************************************************************************
  * @file    onchip_flash.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   CSDN 片上flash管理程序移植
  *
  ******************************************************************************
  * @attention
  *	修改片上Flash大小
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

/*======================================= 移植修改区 =======================================*/

/* FLASH大小：512K */
#define STM32FLASH_SIZE  0x00080000UL

/*======================================= 私有数据区 =======================================*/

/* FLASH起始地址 */
#define STM32FLASH_BASE  FLASH_BASE

/* FLASH结束地址 */
#define STM32FLASH_END   (STM32FLASH_BASE | STM32FLASH_SIZE)

/* FLASH页大小 */
#define STM32FLASH_PAGE_SIZE FLASH_PAGE_SIZE

/* FLASH总页数 */
#define STM32FLASH_PAGE_NUM  (STM32FLASH_SIZE / STM32FLASH_PAGE_SIZE)


/// 导出函数声明
void FLASH_Init(void);
uint32_t FLASH_Read(uint32_t Address, void *Buffer, uint32_t Size);
uint32_t FLASH_Write(uint32_t Address, const uint16_t *Buffer, uint32_t NumToWrite);


#endif // !STMFLASH_H__
