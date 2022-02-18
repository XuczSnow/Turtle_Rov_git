/**
  ******************************************************************************
  * @file    turtle_cam_hk.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 摄像机管理程序
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

#include "stdio.h"

#define CAM_ADD		0x81

Cam_RunStateTypeDef cam_run_state  = CAM_STBY;

void Turtle_Cam_SetFocus(uint8_t dr)
{
	huart[CAM_UART_ID].txBuf[0] = CAM_ADD;
	huart[CAM_UART_ID].txBuf[1] = 0x01;
	huart[CAM_UART_ID].txBuf[2] = 0x04;
	huart[CAM_UART_ID].txBuf[3] = 0x07;
	huart[CAM_UART_ID].txBuf[4] = dr;
	huart[CAM_UART_ID].txBuf[5] = 0xFF;
	huart[CAM_UART_ID].txBufCount = 6;
	HAL_UART_Transmit(&CAM_UART,huart[CAM_UART_ID].txBuf,huart[CAM_UART_ID].txBufCount,0xFFF);
}

/****************字符叠加程序****************/

#define OSD_ID 0x81


char osd_l1[21] = "H:111  P:-00  R:+00 \0";
char osd_l2[21] = "Depth:111.1 m       \0";

uint8_t osd_cmdxy[]  = {OSD_ID,0x01,0x04,0x73,0x11,0x00,0x11,0x11,0x22,0x22,0x00,0x00,0x00,0x00,0x00,0xFF};
uint8_t osd_cmdl1[]  = {OSD_ID,0x01,0x04,0x73,0x21,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0xFF};
uint8_t osd_cmdl2[]  = {OSD_ID,0x01,0x04,0x73,0x31,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0xFF};
uint8_t osd_cmddsp[] = {OSD_ID,0x01,0x04,0x74,0x2F,0xFF};
uint8_t osd_cmdclr[] = {OSD_ID,0x01,0x04,0x74,0x3F,0xFF};
uint8_t osd_length[] = {sizeof(osd_cmdxy),sizeof(osd_cmdl1),sizeof(osd_cmdl2),sizeof(osd_cmddsp),\
												sizeof(osd_cmdxy),sizeof(osd_cmdl1),sizeof(osd_cmdl2),sizeof(osd_cmddsp)};

uint8_t *osd_p[8] = {NULL};

void OSD_Dsp(void)
{
	HAL_UART_Transmit_DMA(&CAM_UART,osd_cmddsp,sizeof(osd_cmddsp));
}

void OSD_Clr(void)
{
	HAL_UART_Transmit_DMA(&CAM_UART,osd_cmdclr,sizeof(osd_cmdclr));
}

void OSD_ReFresh(void)
{
	static uint8_t line_cnt = 0;

	if (line_cnt == 0)
	{
		sprintf(osd_l1,"H:%3d  P:%2d  R:%2d ",(int)hRov.yaw,(int)hRov.pitch,(int)hRov.roll);

		osd_cmdxy[6] = 0x00;
		osd_cmdxy[7] = 0x02;
		osd_cmdxy[8] = 0x00;
		osd_cmdxy[9] = 0x01;
		osd_cmdxy[4] = 0x11;
		osd_cmdl1[4] = 0x21;
		osd_cmdl2[4] = 0x31;
		
		for (uint8_t i=0; i<10; i++)
			osd_cmdl1[5+i] = osd_l1[i], osd_cmdl2[5+i] = osd_l1[10+i];
		
		osd_p[0] = osd_cmdxy;
		osd_p[1] = osd_cmdl1;
		osd_p[2] = osd_cmdl2;
		osd_p[3] = osd_cmddsp;
	}
	else if (line_cnt == 4)
	{
		sprintf(osd_l2,"Depth:%5.1f m       ",(float)hPress.conv_depth[0]/100);
		
		osd_cmdxy[6] = 0x00;
		osd_cmdxy[7] = 0x02;
		osd_cmdxy[8] = 0x00;
		osd_cmdxy[9] = 0x02;
		osd_cmdxy[4] = 0x12;
		osd_cmdl1[4] = 0x22;
		osd_cmdl2[4] = 0x32;
		
		for (uint8_t i=0; i<10; i++)
			osd_cmdl1[5+i] = osd_l2[i], osd_cmdl2[5+i] = osd_l2[10+i];
		
		osd_p[4] = osd_cmdxy;
		osd_p[5] = osd_cmdl1;
		osd_p[6] = osd_cmdl2;
		osd_p[7] = osd_cmddsp;
	}
	
	HAL_UART_Transmit_DMA(&CAM_UART,osd_p[line_cnt],osd_length[line_cnt]);
	
	line_cnt++;
	if (line_cnt == 8)
		line_cnt = 0;
}