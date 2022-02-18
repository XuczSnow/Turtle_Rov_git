/**
  ******************************************************************************
  * @file    turtle_bsp.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV BSP 管理程序
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

/** @defgroup 串口中断处理函数
  * @{
  */


void Turtle_Bsp_Init(void)
{
	//PWM初始化部分
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
	
	//LED状态初始化
	for(uint8_t i=0;i<3;i++)
	{
                               
		Turtle_LED_SetLumin(LED_CH1,30);
		Turtle_LED_SetLumin(LED_CH2,30);
		HAL_Delay(300);
		Turtle_LED_SetLumin(LED_CH1,LED_PWM_MIN);
		Turtle_LED_SetLumin(LED_CH2,LED_PWM_MIN);
		HAL_Delay(300);
	}
	
	Turtle_FlashCheckInit();
	
	//云台初始化
	Turtle_YT_InitAngle();
	hRov.yt_angle = 0;
	
	//推进器初始化
	Turtle_Prop_Init();
	
	//自动控制初始化
	Turtle_AutoCtrl_Init();
	Turtle_SI_Init();
	
	//温湿度传感器初始化
	SHT3X_Init(0x44);
	hSHT30.error = SHT3X_SoftReset();
	HAL_Delay(10);
	
	hSHT30.error |= SHT3X_ReadStatus(&hSHT30.status.u16);
	hSHT30.error = SHT3X_StartPeriodicMeasurment(REPEATAB_HIGH, FREQUENCY_4HZ);
	
	//云台、传感器初始化完成
	Turtle_LED_SetLumin(LED_CH1,30);
	Turtle_LED_SetLumin(LED_CH2,30);
	HAL_Delay(500);
	Turtle_LED_SetLumin(LED_CH1,LED_PWM_MIN);
	Turtle_LED_SetLumin(LED_CH2,LED_PWM_MIN);
	
	//初始化EventRecorder并开启
#if TIMESTAMP_EN == G_TURE
	EventRecorderInitialize(EventRecordAll, 1U);
	EventRecorderStart();
#endif
	
	//推进器定时器初始化部分
	HAL_TIM_Base_Start_IT(&htim5);
	
	//推进器初始化
	for(uint8_t i=0;i<8;i++)
		hprop[i].Prop_CQ = 1600;
	
//	hprop[0].Prop_CQ = 1400;
//	hprop[3].Prop_CQ = 1400;
	
	HAL_Delay(3000);
	
	for(uint8_t i=0;i<8;i++)
		hprop[i].Prop_CQ = PROP_CQ_MID;
	
	//推进器初始化完成指示
	Turtle_LED_SetLumin(LED_CH1,30);
	Turtle_LED_SetLumin(LED_CH2,30);
	HAL_Delay(500);
	Turtle_LED_SetLumin(LED_CH1,LED_PWM_MIN);
	Turtle_LED_SetLumin(LED_CH2,LED_PWM_MIN);
	
	//串口初始化部分
	HAL_UART_Receive_DMA(&huart1, huart[0].rxBuf, RX_DATA_SIZE);//开始接收
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);																//打开空闲中断
	
	HAL_UART_Receive_DMA(&huart2, huart[1].rxBuf, RX_DATA_SIZE);//开始接收
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);																//打开空闲中断
	
	HAL_UART_Receive_DMA(&huart3, huart[2].rxBuf, RX_DATA_SIZE);//开始接收
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);																//打开空闲中断
	
	HAL_UART_Receive_DMA(&huart4, huart[3].rxBuf, RX_DATA_SIZE);//开始接收
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);																//打开空闲中断
	
	HAL_UART_Receive_IT(&huart5, huart[4].rxBuf, RX_DATA_SIZE);	//开始接收
	__HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);																//打开空闲中断
	
	//主机控制初始化
	host[1].err_cnt = 101;
	host[1].joy_stick[KID_RSTICK_FB] = 0x80;
	host[1].joy_stick[KID_RSTICK_RL] = 0x80;
	host[1].joy_stick[KID_LSTICK_FB] = 0x80;
	host[1].joy_stick[KID_LSTICK_RL] = 0x80;
	host[1].joy_stick[KID_RT] = 0x80;
	host[1].joy_stick[KID_LT] = 0x80;
	
	//其余定时器初始化部分
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start_IT(&htim7);
	HAL_TIM_Base_Start_IT(&htim8);
	
//	//初始化压力
//	Turtle_Press_Init();

	hRov.led_lumin1 = LED_PWM_MIN;
	hRov.led_lumin2 = LED_PWM_MIN;

	host[1].prop_gear = 2;
	
	Compass_GetAdd();
	
	//全部初始化完成指示
	Turtle_LED_SetLumin(LED_CH1,30);
	Turtle_LED_SetLumin(LED_CH2,30);
	HAL_Delay(500);
	Turtle_LED_SetLumin(LED_CH1,LED_PWM_MIN);
	Turtle_LED_SetLumin(LED_CH2,LED_PWM_MIN);
}