/**
  ******************************************************************************
  * @file    turtle_press_x101.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 压力传感器读取程序
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

#define PRESS_START_H		0xEB
#define PRESS_START_L		0x90
#define PRESS_END_H			0x0D
#define PRESS_END_L			0x0A
#define PRESS_LENGTH		16u

#define PRESS_CONV_K		1.0336f

Press_StateTypeDef hPress;

void Turtle_Press_Init(void)
{
	uint8_t i=0;
	uint16_t press_buf;
	for(;i<5;)
	{
		if (huart[DPH_UART_ID].rxFlag == 1)
			if (huart[DPH_UART_ID].rxBuf[0] == 0xEB && huart[DPH_UART_ID].rxBuf[1] == 0x90)
				if (huart[DPH_UART_ID].rxBuf[PRESS_LENGTH-2] == 0x0D && huart[DPH_UART_ID].rxBuf[PRESS_LENGTH-1] == 0x0A)
				{
					i++;
					press_buf	+= ((uint16_t)huart[DPH_UART_ID].rxBuf[9]<<8) + huart[DPH_UART_ID].rxBuf[10];
				}

		HAL_Delay(20);
	}
	hPress.init_press = press_buf/5;
}

//低通滤波系数
float lpt_k = 0.2;

void Turtle_Press_DataPro(uint8_t *press_rt)
{
	static uint8_t  press_cnt = 0;
	static uint16_t press_buf = 0;
	static int16_t 	temp_buf = 0;
	static uint8_t	stop_flag = 0;
	
	if (press_rt[0] == 0xEB && press_rt[1] == 0x90)
	{
		if (press_rt[PRESS_LENGTH-2] == 0x0D && press_rt[PRESS_LENGTH-1] == 0x0A)
		{
			if (press_cnt < 5)
			{
				press_buf += ((uint16_t)press_rt[5]<<8) + press_rt[6];
				press_cnt ++;
			}
			else if (press_cnt == 5)
			{
				hPress.init_press = press_buf/5;
				press_cnt ++;
			}
			
			temp_buf = ((uint16_t)press_rt[7]<<8) + press_rt[8];
			
			if (temp_buf>4199)
				stop_flag = 1;
			
			if (stop_flag == 0)
			{
				for(uint8_t i=9;i>0;i--)
				{
					hPress.conv_depth[i] = hPress.conv_depth[i-1];
					hPress.conv_speed[i] = hPress.conv_speed[i-1];
				}
				hPress.press	= ((uint16_t)press_rt[5]<<8) + press_rt[6];
				hPress.temp		= temp_buf;
				hPress.depth	= ((uint16_t)press_rt[9]<<8) + press_rt[10];
				if  (hPress.press > hPress.init_press)
					hPress.conv_depth[0] = (uint16_t)((float)(hPress.press - hPress.init_press)*PRESS_CONV_K);
				else
					hPress.conv_depth[0] = 0;
				
				hPress.conv_speed[0] = (float)(hPress.conv_depth[0] - hPress.conv_depth[1])*(1.0-lpt_k) + hPress.conv_speed[1]*lpt_k;  //单位 mm/s
			}
		}
	}
}