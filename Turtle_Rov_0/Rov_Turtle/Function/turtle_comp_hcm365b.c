/**
  ******************************************************************************
  * @file    turtle_camp_hcm365b.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle ϵ�� ROV ����������������
  *
  ******************************************************************************
  * @attention
  *	
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

uint8_t  BCD2DEC(uint8_t date)
{
	return date - 6 * (date >> 4);//ʮ���ƺ�16���ƽ�λ��Ϊ6��bcd����λ��������6������ԭ������ȥ���λ��
}                               //date >> 4Ϊ���λ����


void Compass_GetAdd(void)
{
	uint8_t i;
//	huart[COMP_UART_ID].txBuf[0] = 0x68;
//	huart[COMP_UART_ID].txBuf[1] = 0x04;
//	huart[COMP_UART_ID].txBuf[2] = 0x00;
//	huart[COMP_UART_ID].txBuf[3] = 0x1F;
//	HAL_UART_Transmit(&COMP_UART,huart[COMP_UART_ID].txBuf,4,0xFF);
//	for (i=0;i<100;i++)
//	{
//		if (huart[COMP_UART_ID].rxFlag == 1)
//		{
//			hRov.comp_id = huart[COMP_UART_ID].rxBuf[2];
//			huart[COMP_UART_ID].rxFlag = 0;
//			break;
//		}
//		HAL_Delay(10);
//	}
	hRov.comp_id = 0x00;
	//��ȡ��ַ�ɹ����Զ����ûظ�Ƶ��
	huart[COMP_UART_ID].txBuf[0] = 0x68;
	huart[COMP_UART_ID].txBuf[1] = 0x05;
	huart[COMP_UART_ID].txBuf[2] = hRov.comp_id;
	huart[COMP_UART_ID].txBuf[3] = 0x0C;
	huart[COMP_UART_ID].txBuf[4] = 0x02;											//���ûظ�Ƶ��15Hz
	huart[COMP_UART_ID].txBuf[5] = 0x00;
	for (i=1;i<5;i++)
		huart[COMP_UART_ID].txBuf[5] += huart[COMP_UART_ID].txBuf[i];
	HAL_UART_Transmit(&COMP_UART,huart[COMP_UART_ID].txBuf,6,0xFF);
	HAL_Delay(10);
}

void Compass_Info(uint8_t *data)
{
	float angbuf;
	if(data[0] == 0x68 || data[0] == 0x77)
	{
		if(data[1] == 0x0D)
		{
			angbuf = (data[4] & 0x0F)*100;
			angbuf = angbuf+BCD2DEC(data[5]);
			angbuf = angbuf+BCD2DEC(data[6])*0.01;
			if ((data[4]>>4) == 0x01)
				angbuf = -angbuf;
			hRov.pitch = angbuf;
			
			angbuf = (data[7] & 0x0F)*100;
			angbuf = angbuf+BCD2DEC(data[8]);
			angbuf = angbuf+BCD2DEC(data[9])*0.01;
			if ((data[7]>>4) == 0x01)
				angbuf = -angbuf;
			hRov.roll = angbuf;
			
			angbuf = (data[10] & 0x0F)*100;
			angbuf = angbuf+BCD2DEC(data[11]);
			angbuf = angbuf+BCD2DEC(data[12])*0.01;
			hRov.yaw = angbuf;
		}
	}
}
