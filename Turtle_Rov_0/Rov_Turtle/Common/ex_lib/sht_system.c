/**
  ******************************************************************************
  * @file    sht_system.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   SHT 系列传感器官方例程中 system.c 文件移植
  *
  ******************************************************************************
  * @attention
  *	根据实际情况修改延时函数
  *
  ******************************************************************************
  */

//=============================================================================
//    S E N S I R I O N   AG,  Laubisruetistr. 50, CH-8712 Staefa, Switzerland
//=============================================================================
// Project   :  SHT3x Sample Code (V1.1)
// File      :  system.c (V1.1)
// Author    :  RFU
// Date      :  6-Mai-2015
// Controller:  STM32F100RB
// IDE       :  矿ision V5.12.0.0
// Compiler  :  Armcc
// Brief     :  System functions
//=============================================================================

//-- Includes -----------------------------------------------------------------
#include "sht_system.h"

//-----------------------------------------------------------------------------
//void SystemInit(void) 
//{
//  // no initialization required
//}

//-----------------------------------------------------------------------------
void DelayMicroSeconds(u32t nbrOfUs)   /* -- adapt this delay for your uC -- */
{
  u32t i;
  for(i = 0; i < nbrOfUs; i++)
  {
		for(uint8_t j=0;j<15;j++)
		{
			__nop();  // nop's may be added or removed for timing adjustment
			__nop();
			__nop();
			__nop();
		}
  }
}
