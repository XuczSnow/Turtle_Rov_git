/**
  ******************************************************************************
  * @file    turtle_yt_j101.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 云台控制程序
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

static float angle;
static uint32_t adc_b;
static uint32_t adc_buf[20];

struct
{
	int32_t yt_min;
	int32_t yt_mid;
	int32_t yt_max;
}YT_ADC_Data;

void Turtle_YT_InitAngle(void)
{
	__HAL_TIM_SET_COMPARE(&YT_TIM,YT_CH,YT_DOWN);
	HAL_Delay(5000);
	__HAL_TIM_SET_COMPARE(&YT_TIM,YT_CH,YT_MID);
	HAL_Delay(5000);
	
	uint16_t while_cnt = 0;
	HAL_ADC_Start_DMA(&hadc1,adc_buf,20);
	__HAL_TIM_SET_COMPARE(&YT_TIM,YT_CH,YT_DOWN);
	HAL_Delay(6000);
	__HAL_TIM_SET_COMPARE(&YT_TIM,YT_CH,YT_MID);
	HAL_Delay(500);
	for(uint8_t i=YT_ADC_CH1;i<20;i=i+2)
		adc_b += adc_buf[i];
	adc_b /= 10;
	YT_ADC_Data.yt_max = adc_b;
	
	adc_b = 0;
	__HAL_TIM_SET_COMPARE(&YT_TIM,YT_CH,YT_UP);
	HAL_Delay(6000);
	__HAL_TIM_SET_COMPARE(&YT_TIM,YT_CH,YT_MID);
	HAL_Delay(500);
	for(uint8_t i=YT_ADC_CH1;i<20;i=i+2)
		adc_b += adc_buf[i];
	adc_b /= 10;
	YT_ADC_Data.yt_min = adc_b;
	
	YT_ADC_Data.yt_mid = (YT_ADC_Data.yt_min + YT_ADC_Data.yt_max)/2;
//	
//	YT_ADC_Data.yt_min = YT_ADC_MIN;
//	YT_ADC_Data.yt_max = YT_ADC_MAX;
//	YT_ADC_Data.yt_mid = (YT_ADC_Data.yt_min + YT_ADC_Data.yt_max)/2;
//		
//	__HAL_TIM_SET_COMPARE(&YT_TIM,YT_CH,YT_DOWN);
//	while((adc_b > YT_ADC_Data.yt_max + YT_ADC_GATE) || (adc_b < YT_ADC_Data.yt_max - YT_ADC_GATE))
//	{
//		adc_b = 0;
//		for(uint8_t i=YT_ADC_CH1;i<20;i=i+2)
//			adc_b += adc_buf[i];
//		adc_b /= 10;
//		HAL_Delay(5);
//		
//		while_cnt ++;
//		if (while_cnt>YT_OUTTIME)
//		{
//			while_cnt = 0;
//			break;
//		}
//	}
//	
//	__HAL_TIM_SET_COMPARE(&YT_TIM,YT_CH,YT_UP);
//	while((adc_b > YT_ADC_Data.yt_min + YT_ADC_GATE) || (adc_b < YT_ADC_Data.yt_min - YT_ADC_GATE))
//	{
//		adc_b = 0;
//		for(uint8_t i=YT_ADC_CH1;i<20;i=i+2)
//			adc_b += adc_buf[i];
//		adc_b /= 10;
//		HAL_Delay(5);
//		
//		while_cnt ++;
//		if (while_cnt>YT_OUTTIME)
//		{
//			while_cnt = 0;
//			break;
//		}
//	}
//	
	__HAL_TIM_SET_COMPARE(&YT_TIM,YT_CH,YT_DOWN);
	while(adc_b > YT_ADC_Data.yt_mid - YT_ADC_GATE)
	{
		adc_b = 0;
		for(uint8_t i=YT_ADC_CH1;i<20;i=i+2)
			adc_b += adc_buf[i];
		adc_b /= 10;
		HAL_Delay(5);
		
		while_cnt ++;
		if (while_cnt>YT_OUTTIME)
		{
			while_cnt = 0;
			break;
		}
	}
	
	__HAL_TIM_SET_COMPARE(&YT_TIM,YT_CH,YT_MID);
	
}

void Turtle_YT_MoveC(uint16_t pwm)
{
	if (hRov.yt_angle >= YT_ANGLE_MIN &&  pwm == YT_DOWN)
		__HAL_TIM_SET_COMPARE(&YT_TIM,YT_CH,pwm);
	else if (hRov.yt_angle <= YT_ANGLE_MAX &&  pwm == YT_UP)
		__HAL_TIM_SET_COMPARE(&YT_TIM,YT_CH,pwm);
	else
		__HAL_TIM_SET_COMPARE(&YT_TIM,YT_CH,YT_MID);
}

int16_t Turtle_YT_GetAngle(void)
{

	for(uint8_t i=YT_ADC_CH1;i<20;i=i+2)
		adc_b += adc_buf[i];
	adc_b /= 10;
	angle = ((float)adc_b - YT_ADC_Data.yt_mid)/(YT_ADC_Data.yt_max-YT_ADC_Data.yt_mid)*YT_ANGLE_MMAX;
	adc_b = 0;
	
	return (int16_t)(-angle*10);
}