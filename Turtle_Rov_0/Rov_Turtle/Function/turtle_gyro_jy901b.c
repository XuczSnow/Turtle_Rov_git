/**
  ******************************************************************************
  * @file    turtle_gyro_jy901b.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   JY901B ϵ�д������ٷ��������ļ���ֲ
  *
  ******************************************************************************
  * @attention
  * ����ԭ��ע�Ͱ�Ȩ��Ϣ������ֻ�����˽�����ص�����
  *
  ******************************************************************************
  */

/*
	��д�ߣ�Kevin
	��ַ��http://RobotControl.taobao.com
	����E-mail��1609370741@qq.com
	���뻷����MDK-Lite  Version: 5.17
	����ʱ��: 2016-1-31
	���ԣ� ���������ڡ������ǿء���STM32Coreƽ̨����ɲ���
*/
 
/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"
#include "string.h"

static struct STime			stcTime;
static struct SAcc 			stcAcc;
static struct SGyro 		stcGyro;
static struct SAngle 		stcAngle;
static struct SMag 			stcMag;
static struct SDStatus 	stcDStatus;
static struct SPress 		stcPress;
static struct SLonLat 	stcLonLat;
static struct SGPSV 		stcGPSV;
static struct SQ       	stcQ;

//CopeSerialDataΪ����2�жϵ��ú���������ÿ�յ�һ�����ݣ�����һ�����������
void CopeSerial2Data(uint8_t *ucData)
{
	if (ucData[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		return;
	}
	else
	{
		switch(ucData[1])//�ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ���У���Щ���ݰ���Ҫͨ����λ���򿪶�Ӧ������󣬲��ܽ��յ�������ݰ�������
		{
			case 0x50:	memcpy(&stcTime,&ucData[2],8);break;
			case 0x51:	memcpy(&stcAcc,&ucData[2],8);break;
			case 0x52:	memcpy(&stcGyro,&ucData[2],8);break;
			case 0x53:
				memcpy(&stcAngle,&ucData[2],8);
				hRov.roll		= (float)stcAngle.Angle[0]/32768*180; 	//ROLL
				hRov.pitch	= (float)stcAngle.Angle[1]/32768*180;		//PITCH
				hRov.yaw		= (float)stcAngle.Angle[2]/32768*180;		//YAW
				//����ϵת��
				hRov.yaw = 180.0-hRov.yaw;
				//��װʱת��
//				hRov.pitch = -hRov.pitch;
//				if (hRov.roll>0)
//					hRov.roll = 180 - hRov.roll;
//				else
//					hRov.roll = 180 + hRov.roll;
				break;
			case 0x54:	memcpy(&stcMag,&ucData[2],8);break;
			case 0x55:	memcpy(&stcDStatus,&ucData[2],8);break;
			case 0x56:	memcpy(&stcPress,&ucData[2],8);break;
			case 0x57:	memcpy(&stcLonLat,&ucData[2],8);break;
			case 0x58:	memcpy(&stcGPSV,&ucData[2],8);break;
			case 0x59:	memcpy(&stcQ,&ucData[2],8);break;
		}
	}
}
