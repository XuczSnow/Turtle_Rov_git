/**
  ******************************************************************************
  * @file    turtle_temp_nta8a01_ttl.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 电源温度传感器读取程序
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

#define TEMP_ADD		0x01

void Temp_Check(void)
{
	uint16_t crc16_buf;
	
	huart[TEMP_UART_ID].txBuf[0] = TEMP_ADD;
	huart[TEMP_UART_ID].txBuf[1] = 0x03;
	huart[TEMP_UART_ID].txBuf[2] = 0x00;
	huart[TEMP_UART_ID].txBuf[3] = 0x00;
	huart[TEMP_UART_ID].txBuf[4] = 0x00;
	huart[TEMP_UART_ID].txBuf[5] = 0x01;
	crc16_buf = CRC16_MODBUS(huart[TEMP_UART_ID].txBuf,6);
	huart[TEMP_UART_ID].txBuf[6] = crc16_buf;
	huart[TEMP_UART_ID].txBuf[7] = crc16_buf>>8;
	huart[TEMP_UART_ID].txBufCount = 8;
	HAL_UART_Transmit(&TEMP_UART,huart[TEMP_UART_ID].txBuf,huart[TEMP_UART_ID].txBufCount,0xFFF);
}

int16_t Temp_DataPro(uint8_t *temp_rt)
{
	if (temp_rt[0] == TEMP_ADD && temp_rt[1] == 03 && temp_rt[2] == 0x02)
	{
		return ((uint16_t)temp_rt[3]<<8) + temp_rt[4];
	}
	else
		return 0;
}