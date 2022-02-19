/**
  ******************************************************************************
  * @file    turtle_press_rovmaker.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   RovMaker 系列 ROV 压力传感器读取程序
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#include "turtle_g.h"

void Turtle_Press_RM(uint8_t *data, Press_StateTypeDef *hp)
{
	float			buf[2] = {0};
	uint8_t		cnt;
	
	for(uint8_t i=0;; i++)
	{
		if (data[i] == 'T') cnt = 0;
		else if (data[i] == 'D') cnt = 1;
		
		else if (data[i] >= '0' && data[i] <= '9')
		{
			buf[cnt] = buf[cnt]*10 + (data[i]-'0');
		}
		
		else if (data[i] == '.') ;
		else break;
	}
	hp->temp = buf[1];
	hp->depth = buf[0]/10;
}
