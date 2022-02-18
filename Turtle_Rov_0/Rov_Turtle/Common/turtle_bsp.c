/**
  ******************************************************************************
  * @file    turtle_bsp.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle ϵ�� ROV BSP �������
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

/** @defgroup �����жϴ�����
  * @{
  */


void Turtle_Bsp_Init(void)
{
	//PWM��ʼ������
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
	
	//LED״̬��ʼ��
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
	
	//��̨��ʼ��
	Turtle_YT_InitAngle();
	hRov.yt_angle = 0;
	
	//�ƽ�����ʼ��
	Turtle_Prop_Init();
	
	//�Զ����Ƴ�ʼ��
	Turtle_AutoCtrl_Init();
	Turtle_SI_Init();
	
	//��ʪ�ȴ�������ʼ��
	SHT3X_Init(0x44);
	hSHT30.error = SHT3X_SoftReset();
	HAL_Delay(10);
	
	hSHT30.error |= SHT3X_ReadStatus(&hSHT30.status.u16);
	hSHT30.error = SHT3X_StartPeriodicMeasurment(REPEATAB_HIGH, FREQUENCY_4HZ);
	
	//��̨����������ʼ�����
	Turtle_LED_SetLumin(LED_CH1,30);
	Turtle_LED_SetLumin(LED_CH2,30);
	HAL_Delay(500);
	Turtle_LED_SetLumin(LED_CH1,LED_PWM_MIN);
	Turtle_LED_SetLumin(LED_CH2,LED_PWM_MIN);
	
	//��ʼ��EventRecorder������
#if TIMESTAMP_EN == G_TURE
	EventRecorderInitialize(EventRecordAll, 1U);
	EventRecorderStart();
#endif
	
	//�ƽ�����ʱ����ʼ������
	HAL_TIM_Base_Start_IT(&htim5);
	
	//�ƽ�����ʼ��
	for(uint8_t i=0;i<8;i++)
		hprop[i].Prop_CQ = 1600;
	
//	hprop[0].Prop_CQ = 1400;
//	hprop[3].Prop_CQ = 1400;
	
	HAL_Delay(3000);
	
	for(uint8_t i=0;i<8;i++)
		hprop[i].Prop_CQ = PROP_CQ_MID;
	
	//�ƽ�����ʼ�����ָʾ
	Turtle_LED_SetLumin(LED_CH1,30);
	Turtle_LED_SetLumin(LED_CH2,30);
	HAL_Delay(500);
	Turtle_LED_SetLumin(LED_CH1,LED_PWM_MIN);
	Turtle_LED_SetLumin(LED_CH2,LED_PWM_MIN);
	
	//���ڳ�ʼ������
	HAL_UART_Receive_DMA(&huart1, huart[0].rxBuf, RX_DATA_SIZE);//��ʼ����
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);																//�򿪿����ж�
	
	HAL_UART_Receive_DMA(&huart2, huart[1].rxBuf, RX_DATA_SIZE);//��ʼ����
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);																//�򿪿����ж�
	
	HAL_UART_Receive_DMA(&huart3, huart[2].rxBuf, RX_DATA_SIZE);//��ʼ����
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);																//�򿪿����ж�
	
	HAL_UART_Receive_DMA(&huart4, huart[3].rxBuf, RX_DATA_SIZE);//��ʼ����
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);																//�򿪿����ж�
	
	HAL_UART_Receive_IT(&huart5, huart[4].rxBuf, RX_DATA_SIZE);	//��ʼ����
	__HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);																//�򿪿����ж�
	
	//�������Ƴ�ʼ��
	host[1].err_cnt = 101;
	host[1].joy_stick[KID_RSTICK_FB] = 0x80;
	host[1].joy_stick[KID_RSTICK_RL] = 0x80;
	host[1].joy_stick[KID_LSTICK_FB] = 0x80;
	host[1].joy_stick[KID_LSTICK_RL] = 0x80;
	host[1].joy_stick[KID_RT] = 0x80;
	host[1].joy_stick[KID_LT] = 0x80;
	
	//���ඨʱ����ʼ������
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start_IT(&htim7);
	HAL_TIM_Base_Start_IT(&htim8);
	
//	//��ʼ��ѹ��
//	Turtle_Press_Init();

	hRov.led_lumin1 = LED_PWM_MIN;
	hRov.led_lumin2 = LED_PWM_MIN;

	host[1].prop_gear = 2;
	
	Compass_GetAdd();
	
	//ȫ����ʼ�����ָʾ
	Turtle_LED_SetLumin(LED_CH1,30);
	Turtle_LED_SetLumin(LED_CH2,30);
	HAL_Delay(500);
	Turtle_LED_SetLumin(LED_CH1,LED_PWM_MIN);
	Turtle_LED_SetLumin(LED_CH2,LED_PWM_MIN);
}