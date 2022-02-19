/**
  ******************************************************************************
  * @file    turtle_gyro_jy901b.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   JY901B 系列传感器官方例程中文件移植
  *
  ******************************************************************************
  * @attention
  * 保留原版注释版权信息，程序只采用了解析相关的内容
  *
  ******************************************************************************
  */

/*
	编写者：Kevin
	网址：http://RobotControl.taobao.com
	作者E-mail：1609370741@qq.com
	编译环境：MDK-Lite  Version: 5.17
	初版时间: 2016-1-31
	测试： 本程序已在【君悦智控】的STM32Core平台上完成测试
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

//CopeSerialData为串口2中断调用函数，串口每收到一个数据，调用一次这个函数。
void CopeSerial2Data(uint8_t *ucData)
{
	if (ucData[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
	{
		return;
	}
	else
	{
		switch(ucData[1])//判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
		{
			case 0x50:	memcpy(&stcTime,&ucData[2],8);break;
			case 0x51:	memcpy(&stcAcc,&ucData[2],8);break;
			case 0x52:	memcpy(&stcGyro,&ucData[2],8);break;
			case 0x53:
				memcpy(&stcAngle,&ucData[2],8);
				hRov.roll		= (float)stcAngle.Angle[0]/32768*180; 	//ROLL
				hRov.pitch	= (float)stcAngle.Angle[1]/32768*180;		//PITCH
				hRov.yaw		= (float)stcAngle.Angle[2]/32768*180;		//YAW
				//坐标系转换
				hRov.yaw = 180.0-hRov.yaw;
				//反装时转换
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
