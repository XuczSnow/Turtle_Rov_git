/**
  ******************************************************************************
  * @file    turtle_it.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle ϵ�� ROV �жϹ������
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
UART_DataTypeDef huart[5];

//��ʱ������ʹ����̬����
uint8_t at_en = 0;

void USER_UART_IdleCpltCallback(UART_HandleTypeDef *huart_arg, UART_IDTypeDef uart_id)
{
	uint32_t temp;
	if((__HAL_UART_GET_FLAG(huart_arg, UART_FLAG_IDLE) != RESET))
	{
		if (huart_arg->Instance != UART5)
		{
			__HAL_UART_CLEAR_IDLEFLAG(huart_arg);
			HAL_UART_DMAStop(huart_arg);
			temp = huart_arg->hdmarx->Instance->CNDTR;
			huart[uart_id].rxBufCount =  RX_DATA_SIZE - temp;
			HAL_UART_Receive_DMA(huart_arg, huart[uart_id].rxBuf, RX_DATA_SIZE);
			HAL_UART_DMAResume(huart_arg);
		}
		else if (huart_arg->Instance == UART5)
		{
			__HAL_UART_CLEAR_IDLEFLAG(huart_arg);
			HAL_UART_Receive_IT(huart_arg, huart[uart_id].rxBuf, RX_DATA_SIZE);
		}
		huart[uart_id].rxFlag = 1;
		
		//�����жϹ��ܴ�����
		if(huart_arg->Instance == HOST_UART.Instance)
		{
			if (huart[uart_id].rxBuf[0] != 0xEB)
			{
				for (uint8_t i=0; i<huart[uart_id].rxBufCount; i++)
					QueueIn(&Terminal_RxQueue,huart[uart_id].rxBuf[i]);
			}
			else
			{
				//����������
				Turtle_HostData(huart[HOST_UART_ID].rxBuf);
				host[1].err_cnt = 0;
			}
			
			huart[HOST_UART_ID].rxFlag = 0;
		}
		else if (huart_arg->Instance == DPH_UART.Instance)
		{
			//��ȴ���������
			Turtle_Press_DataPro(huart[DPH_UART_ID].rxBuf);
			huart[DPH_UART_ID].rxFlag = 0;
		}
		else if (huart_arg->Instance == PROP_UART.Instance)
		{
			//�ƽ������ݴ���
			Turtle_Prop_DataPro(huart[PROP_UART_ID].rxBuf);
			huart[PROP_UART_ID].rxFlag = 0;
		}
		else if (huart_arg->Instance == GYRO_UART.Instance)
		{
#if G_ANGLE == G_GYRO
			//���������ݴ���
			CopeSerial2Data(huart[GYRO_UART_ID].rxBuf);
#elif G_ANGLE == G_COMP
			//�����������ݴ���
			Compass_Info(huart[COMP_UART_ID].rxBuf);
#endif
			huart[GYRO_UART_ID].rxFlag = 0;
		}
		else if (huart_arg->Instance == TEMP_UART.Instance)
		{
			//�¶ȴ��������ݴ���
			//(�˴�Ϊ��)
		}
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART3)
		HAL_GPIO_WritePin(P485_EN_GPIO_Port,P485_EN_Pin,GPIO_PIN_RESET);
}

/** @defgroup ��ʱ���жϴ�����
  * @{
  */

uint8_t auto_cycle = 20;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == PROPC_TIM.Instance)
	{
		//�ƽ�������
		Turtle_Prop_Update();
	}
	else if (htim->Instance == ACTRL_TIM.Instance)
	{
		static uint8_t actrl_cnt = 0;
		
		actrl_cnt++;
		
		if (actrl_cnt % 10 == 0)
		{
			actrl_cnt = 0;
			//�˶�����
			host[1].err_cnt ++;
			
			uint8_t* p_buf;
			//��λ��ͨѶ������
			if (host[1].err_cnt >= 100)
			{
				//��ֵ����
				p_buf = host[1].joy_stick;
					for(uint8_t i=0;i<20;i++)
						p_buf[i] = 0x00;
				
				host[1].joy_stick[KID_RSTICK_FB] = 0x80;
				host[1].joy_stick[KID_RSTICK_RL] = 0x80;
				host[1].joy_stick[KID_LSTICK_FB] = 0x80;
				host[1].joy_stick[KID_LSTICK_RL] = 0x80;
				host[1].joy_stick[KID_RT] = 0x80;
				host[1].joy_stick[KID_LT] = 0x80;
				
				host[1].err_cnt = 100;
			}
			
			if (host[1].joy_stick[KID_RSTICK] == 0x00 && host[1].joy_stick[KID_LSTICK] == 0x00)
				//�ֶ�����
				Turtle_HostMove();
			else if (host[1].joy_stick[KID_RSTICK] == 0x01 && host[1].joy_stick[KID_LSTICK] == 0x01)
				//�ֶ�����
				;
			else if (host[1].joy_stick[KID_RSTICK] == 0x02 && host[1].joy_stick[KID_LSTICK] == 0x02)
				//ֱ�ӿ����ƽ���
				Turtle_HostCtrlProp();
			else
				;
			
			//�̺���
				//ֱ�ӿ��ƺ�����ˮƽ�ƽ���
			
				//���򲹳�
			
			
			if (actrl_cnt % auto_cycle == 0)
			{
				//�Զ�����
				Turtle_AutoCtrl_Refresh();
				for(uint8_t i=0;i<4;i++)
					Turtle_AutoCtrl(&hAuto[i]);
				for(uint8_t i=0;i<4;i++)
					Turtle_SI(&hSI[i]);
			}
			
			Turtle_Prop_GatePro();
		}
	}
	else if (htim->Instance == LSTASK_TIM.Instance)
	{
		//����������ȿ�����
		Turtle_Timer_LSTask();
	}
	else if (htim->Instance == HSTASK_TIM.Instance)
	{
		//����������ȿ�����
	}
}