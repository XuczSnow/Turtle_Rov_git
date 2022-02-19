/**
  ******************************************************************************
  * @file    turtle_lsjc_gpio.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 漏水检测程序
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

LSJC_StateTypeDef Turtle_LSJC(uint8_t ch)
{
	switch(ch)
	{
		case LSJC_CH1:
			if(HAL_GPIO_ReadPin(LSJC_1_GPIO_Port,LSJC_1_Pin))
				return LSJC_NORM;
			else
				return LSJC_LS;
			break;
		case LSJC_CH2:
			if(HAL_GPIO_ReadPin(LSJC_2_GPIO_Port,LSJC_2_Pin))
				return LSJC_NORM;
			else
				return LSJC_LS;
			break;			
	}

	return LSJC_NONE;
}
