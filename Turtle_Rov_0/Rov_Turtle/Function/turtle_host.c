/**
  ******************************************************************************
  * @file    turtle_host.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 上位机通信处理程序
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

#define HOST_START_H		0xEB
#define HOST_START_L		0x90
#define HOST_END_H			0x0D
#define HOST_END_L			0x0A
#define HOST_LENGTH			64u
#define HOST_FLENGTH		64u

Rov_StateTypeDef	hRov = {
	.prop_cq_gmax = PROP_CQ_GMAX,
};
Host_StateTypeDef host[2];

float gear_switch[4] = {0,2,1.5,1};  //挡位系数设置

static uint8_t AbsMinus(uint8_t arg1, uint8_t arg2)
{
	if (arg1>arg2)
		return arg1-arg2;
	else
		return arg2-arg1;
}

static uint8_t KeyValue_Div(uint8_t key_value, float arg)
{
	return (uint8_t)(KEY_MID + ((float)key_value-KEY_MID)/arg);
}
	
static void ShortConvertSign(short input,uint8_t *buf)
{
	uint8_t sign;
	
	if(input < 0)
	{
		sign = 0x01;
		input = -input;
	}
	else
		sign = 0x00;
	
	buf[0] = (uint8_t)(((uint16_t)input)>>8);
	buf[0] = buf[0]|(sign<<7);
	buf[1] = (uint8_t)((uint16_t)input);
}

#if 0 //旧的操作方式
void Turtle_HostMove(void)
{
	//推进器控制
		//水平推进器控制
	if (host[1].joy_stick[KID_RB] || ((host[1].auto_flag&hAuto[AUTO_DEPTH_ID].symbol) == hAuto[AUTO_DEPTH_ID].symbol\
																&& (host[1].auto_flag&hAuto[AUTO_HEADING_ID].symbol) != hAuto[AUTO_HEADING_ID].symbol))
	{
//		if (AbsMinus(host[1].joy_stick[KID_RSTICK_FB],KEY_MID) < KEY_MAX_GATE && AbsMinus(host[1].joy_stick[KID_RSTICK_RL],KEY_MID) > KEY_MAX_GATE)
//			Turtle_Prop_Side(KeyValue_Div(host[1].joy_stick[KID_RSTICK_RL],2));
//		else
		{
			Turtle_Prop_FwBack(KEY_MAX - host[1].joy_stick[KID_RSTICK_FB]);
			Turtle_Prop_Heading(KeyValue_Div(host[1].joy_stick[KID_RSTICK_RL],1.5));
		}
		Turtle_AutoCtrl_EnAt();
		hAuto[AUTO_HEADING_ID].enable = AUTO_DISABLE;
		hSI[AUTO_HEADING_ID].enable = AUTO_DISABLE;
	}
	else if (host[1].joy_stick[KID_RT] < 0x70)
	{
		Turtle_Prop_Rotation(KeyValue_Div(host[1].joy_stick[KID_RSTICK_RL],1.5));
		Turtle_AutoCtrl_EnAt();
		hAuto[AUTO_HEADING_ID].enable = AUTO_DISABLE;
		hSI[AUTO_HEADING_ID].enable = AUTO_DISABLE;
	}
	else if (host[1].joy_stick[KID_LB])
	{
		Turtle_Prop_Side(KeyValue_Div(host[1].joy_stick[KID_RSTICK_RL],2));
		Turtle_AutoCtrl_EnAt();
		hAuto[AUTO_HEADING_ID].enable = AUTO_DISABLE;
		hSI[AUTO_HEADING_ID].enable = AUTO_DISABLE;
	}
	else if((host[1].auto_flag&hAuto[AUTO_HEADING_ID].symbol) == hAuto[AUTO_HEADING_ID].symbol)
	{
		Turtle_Prop_FwBack(KEY_MAX - host[1].joy_stick[KID_RSTICK_FB]);
		Turtle_AutoCtrl_EnAt();
		hAuto[AUTO_HEADING_ID].enable = AUTO_ENABLE;
		hSI[AUTO_HEADING_ID].enable = AUTO_DISABLE;
	}
	else if((host[1].si_flag&hSI[AUTO_HEADING_ID].mode) == hSI[AUTO_HEADING_ID].mode)
	{
		Turtle_AutoCtrl_DisAt();
		hSI[AUTO_HEADING_ID].enable = AUTO_ENABLE;
		hAuto[AUTO_HEADING_ID].enable = AUTO_DISABLE;
	}
	else
	{
		Turtle_Prop_PCLC();
		Turtle_AutoCtrl_DisAt();
		hAuto[AUTO_HEADING_ID].esum = 0;
		hAuto[AUTO_PITCH_ID].esum = 0;
		hAuto[AUTO_ROLL_ID].esum = 0;
		hAuto[AUTO_HEADING_ID].enable = AUTO_DISABLE;
		hSI[AUTO_HEADING_ID].enable = AUTO_DISABLE;
	}
	
	
		//垂向推进器控制
	if (host[1].joy_stick[KID_RB] || ((host[1].auto_flag&hAuto[AUTO_HEADING_ID].symbol) == hAuto[AUTO_HEADING_ID].symbol\
																&&	(host[1].auto_flag&hAuto[AUTO_DEPTH_ID].symbol) != hAuto[AUTO_DEPTH_ID].symbol))
	{
		Turtle_Prop_UpDown(host[1].joy_stick[KID_LSTICK_FB]);
		Turtle_AutoCtrl_EnAt();
		hAuto[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_PITCH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_ROLL_ID].enable	= AUTO_DISABLE;
	}
	else if (host[1].joy_stick[KID_LT] > 0x90)
	{
		if (AbsMinus(host[1].joy_stick[KID_LSTICK_FB],KEY_MID) > AbsMinus(host[1].joy_stick[KID_LSTICK_RL],KEY_MID))
		{
			Turtle_Prop_Pitch(host[1].joy_stick[KID_LSTICK_FB]);
		}
		else
			Turtle_Prop_Roll(host[1].joy_stick[KID_LSTICK_RL]);
		
		Turtle_AutoCtrl_DisAt();
		hAuto[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_PITCH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_ROLL_ID].enable	= AUTO_DISABLE;
	}
	else if ((host[1].auto_flag&hAuto[AUTO_DEPTH_ID].symbol) == hAuto[AUTO_DEPTH_ID].symbol)
	{
		Turtle_AutoCtrl_EnAt();
		hAuto[AUTO_DEPTH_ID].enable = AUTO_ENABLE;
		hSI[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_PITCH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_ROLL_ID].enable	= AUTO_DISABLE;
	}
	else if((host[1].si_flag&hSI[AUTO_DEPTH_ID].mode) == hSI[AUTO_DEPTH_ID].mode)
	{
		Turtle_AutoCtrl_DisAt();
		hAuto[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_DEPTH_ID].enable = AUTO_ENABLE;
		hSI[AUTO_PITCH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_ROLL_ID].enable	= AUTO_DISABLE;
	}
	else if((host[1].si_flag&hSI[AUTO_PITCH_ID].mode) == hSI[AUTO_PITCH_ID].mode)
	{
		Turtle_AutoCtrl_DisAt();
		hAuto[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_PITCH_ID].enable = AUTO_ENABLE;
		hSI[AUTO_ROLL_ID].enable	= AUTO_DISABLE;
	}
	else if((host[1].si_flag&hSI[AUTO_ROLL_ID].mode) == hSI[AUTO_ROLL_ID].mode)
	{
		Turtle_AutoCtrl_DisAt();
		hAuto[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_PITCH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_ROLL_ID].enable	= AUTO_ENABLE;
	}
	else
	{
		Turtle_Prop_VCLC();
//		Turtle_AutoCtrl_DisAt();
		hAuto[AUTO_DEPTH_ID].esum = 0;
		hAuto[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_PITCH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_ROLL_ID].enable	= AUTO_DISABLE;
	}
	
	//自动控制状态检查
	uint8_t bit_buf1 = 0x80;
	for (uint8_t i=0;i<3;i++)
	{
		//不使能姿态时，屏蔽标志位
		if (at_en == 0 && i ==2)
		{
			hRov.run_state &= ~(bit_buf1>>i);
			continue;
		}
		
		if (hAuto[i].enable == AUTO_ENABLE)
			hRov.run_state |= bit_buf1>>i;
		else
			hRov.run_state &= ~(bit_buf1>>i);
	}
}
#else
void Turtle_HostMove(void)
{
	//推进器控制
		//水平推进器控制
	if (AbsMinus(host[1].joy_stick[KID_RSTICK_FB],KEY_MID) > KEY_MIN_GATE || AbsMinus(host[1].joy_stick[KID_LSTICK_RL],KEY_MID) > KEY_MIN_GATE)
	{
		Turtle_Prop_FwBack(KEY_MAX - host[1].joy_stick[KID_RSTICK_FB]);
		Turtle_Prop_Heading(KeyValue_Div(host[1].joy_stick[KID_LSTICK_RL],1.5));
		Turtle_AutoCtrl_EnAt();
		hAuto[AUTO_HEADING_ID].enable = AUTO_DISABLE;
		hSI[AUTO_HEADING_ID].enable = AUTO_DISABLE;
	}
	
	else if (host[1].joy_stick[KID_LB] || host[1].joy_stick[KID_RB])
	{
		if (host[1].joy_stick[KID_LB])
			Turtle_Prop_Rotation(KeyValue_Div(0x0F,host[1].prop_gear));
		else
			Turtle_Prop_Rotation(KeyValue_Div(0xF0,host[1].prop_gear));
		Turtle_AutoCtrl_EnAt();
		hAuto[AUTO_HEADING_ID].enable = AUTO_DISABLE;
		hSI[AUTO_HEADING_ID].enable = AUTO_DISABLE;
	}
//	else if (host[1].joy_stick[KID_LB])
//	{
//		Turtle_Prop_Side(KeyValue_Div(host[1].joy_stick[KID_RSTICK_RL],2));
//		Turtle_AutoCtrl_EnAt();
//		hAuto[AUTO_HEADING_ID].enable = AUTO_DISABLE;
//		hSI[AUTO_HEADING_ID].enable = AUTO_DISABLE;
//	}
	else if((host[1].auto_flag&hAuto[AUTO_HEADING_ID].symbol) == hAuto[AUTO_HEADING_ID].symbol)
	{
		Turtle_Prop_FwBack(KEY_MAX - host[1].joy_stick[KID_RSTICK_FB]);
		Turtle_AutoCtrl_EnAt();
		hAuto[AUTO_HEADING_ID].enable = AUTO_ENABLE;
		hSI[AUTO_HEADING_ID].enable = AUTO_DISABLE;
	}
	else if((host[1].si_flag&hSI[AUTO_HEADING_ID].mode) == hSI[AUTO_HEADING_ID].mode)
	{
		Turtle_AutoCtrl_DisAt();
		hSI[AUTO_HEADING_ID].enable = AUTO_ENABLE;
		hAuto[AUTO_HEADING_ID].enable = AUTO_DISABLE;
	}
	else
	{
		Turtle_Prop_PCLC();
		Turtle_AutoCtrl_DisAt();
		hAuto[AUTO_HEADING_ID].esum = 0;
		hAuto[AUTO_PITCH_ID].esum = 0;
		hAuto[AUTO_ROLL_ID].esum = 0;
		hAuto[AUTO_HEADING_ID].enable = AUTO_DISABLE;
		hSI[AUTO_HEADING_ID].enable = AUTO_DISABLE;
	}
	
	
		//垂向推进器控制
	if (host[1].joy_stick[KID_LT] > KEY_MID+KEY_MIN_GATE || host[1].joy_stick[KID_RT] < KEY_MID-KEY_MIN_GATE)
	{
		if (host[1].joy_stick[KID_LT] > KEY_MID+KEY_MIN_GATE)
		{
			if (host[1].auto_flag&hAuto[AUTO_DEPTH_ID].symbol)
				Turtle_Prop_UpDown(host[1].joy_stick[KID_LT]/4+hAuto[AUTO_DEPTH_ID].pidout); //手动跟随并限制速度
			else
				Turtle_Prop_UpDown(host[1].joy_stick[KID_LT]);
		}
		else
		{
			if (host[1].auto_flag&hAuto[AUTO_DEPTH_ID].symbol)
				Turtle_Prop_UpDown(host[1].joy_stick[KID_RT]/4+hAuto[AUTO_DEPTH_ID].pidout); //手动跟随并限制速度
			else
				Turtle_Prop_UpDown(host[1].joy_stick[KID_RT]);
		}
		Turtle_AutoCtrl_EnAt();
		
					
		//深度控制跟随
#if DEPTH_FLW == 1u
		if (host[1].auto_flag&hAuto[AUTO_DEPTH_ID].symbol)
		{
			hAuto[AUTO_DEPTH_ID].set_value = (float)hPress.conv_depth[0]*0.1;
		}
#endif
		
		hAuto[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_PITCH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_ROLL_ID].enable	= AUTO_DISABLE;
	}
//	else if (host[1].joy_stick[KID_LT] > 0x90)
//	{
//		if (AbsMinus(host[1].joy_stick[KID_LSTICK_FB],KEY_MID) > AbsMinus(host[1].joy_stick[KID_LSTICK_RL],KEY_MID))
//		{
//			Turtle_Prop_Pitch(host[1].joy_stick[KID_LSTICK_FB]);
//		}
//		else
//			Turtle_Prop_Roll(host[1].joy_stick[KID_LSTICK_RL]);
//		
//		Turtle_AutoCtrl_DisAt();
//		hAuto[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
//		hSI[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
//		hSI[AUTO_PITCH_ID].enable = AUTO_DISABLE;
//		hSI[AUTO_ROLL_ID].enable	= AUTO_DISABLE;
//	}
	else if ((host[1].auto_flag&hAuto[AUTO_DEPTH_ID].symbol) == hAuto[AUTO_DEPTH_ID].symbol)
	{
		Turtle_AutoCtrl_EnAt();
		hAuto[AUTO_DEPTH_ID].enable = AUTO_ENABLE;
		hSI[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_PITCH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_ROLL_ID].enable	= AUTO_DISABLE;
	}
	else if((host[1].si_flag&hSI[AUTO_DEPTH_ID].mode) == hSI[AUTO_DEPTH_ID].mode)
	{
		Turtle_AutoCtrl_DisAt();
		hAuto[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_DEPTH_ID].enable = AUTO_ENABLE;
		hSI[AUTO_PITCH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_ROLL_ID].enable	= AUTO_DISABLE;
	}
	else if((host[1].si_flag&hSI[AUTO_PITCH_ID].mode) == hSI[AUTO_PITCH_ID].mode)
	{
		Turtle_AutoCtrl_DisAt();
		hAuto[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_PITCH_ID].enable = AUTO_ENABLE;
		hSI[AUTO_ROLL_ID].enable	= AUTO_DISABLE;
	}
	else if((host[1].si_flag&hSI[AUTO_ROLL_ID].mode) == hSI[AUTO_ROLL_ID].mode)
	{
		Turtle_AutoCtrl_DisAt();
		hAuto[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_PITCH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_ROLL_ID].enable	= AUTO_ENABLE;
	}
	else
	{
		Turtle_Prop_VCLC();
		hAuto[AUTO_DEPTH_ID].esum = 0;
		hAuto[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_DEPTH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_PITCH_ID].enable = AUTO_DISABLE;
		hSI[AUTO_ROLL_ID].enable	= AUTO_DISABLE;
	}
	
	//自动控制状态检查
	uint8_t bit_buf1 = 0x80;
	for (uint8_t i=0;i<3;i++)
	{
		//不使能姿态时，屏蔽标志位
		if (at_en == 0 && i ==2)
		{
			hRov.run_state &= ~(bit_buf1>>i);
			continue;
		}
		
		if (hAuto[i].enable == AUTO_ENABLE)
			hRov.run_state |= bit_buf1>>i;
		else
			hRov.run_state &= ~(bit_buf1>>i);
	}
}
#endif

void Turtle_HostFb(void)
{
	uint8_t  buf[2];
	uint16_t u16_buf;
	uint32_t u32_dsp;

	huart[HOST_UART_ID].txBuf[0] = 0xEB;
	huart[HOST_UART_ID].txBuf[1] = 0x90;
	huart[HOST_UART_ID].txBuf[2] = HOST_FLENGTH;
	//状态反馈未加
	huart[HOST_UART_ID].txBuf[3] = hRov.run_state>>16;
	huart[HOST_UART_ID].txBuf[4] = hRov.run_state>>8;
	huart[HOST_UART_ID].txBuf[5] = hRov.run_state;

	ShortConvertSign((short)(hRov.yaw*10),buf);
	huart[HOST_UART_ID].txBuf[6] = buf[0];
	huart[HOST_UART_ID].txBuf[7] = buf[1];

	ShortConvertSign((short)(hRov.roll*10),buf);
	huart[HOST_UART_ID].txBuf[8] = buf[0];
	huart[HOST_UART_ID].txBuf[9] = buf[1];

	ShortConvertSign((short)(hRov.pitch*10),buf);
	huart[HOST_UART_ID].txBuf[10]= buf[0];
	huart[HOST_UART_ID].txBuf[11]= buf[1];

	ShortConvertSign((short)(hPress.temp/10),buf);
	huart[HOST_UART_ID].txBuf[12]= buf[0];
	huart[HOST_UART_ID].txBuf[13]= buf[1];

	ShortConvertSign((short)(hSHT30.temperature*10),buf);
	huart[HOST_UART_ID].txBuf[14]= buf[0];
	huart[HOST_UART_ID].txBuf[15]= buf[1];

	ShortConvertSign((short)(hPress.conv_depth[0]),buf);
	huart[HOST_UART_ID].txBuf[16]= buf[0];
	huart[HOST_UART_ID].txBuf[17]= buf[1];

	huart[HOST_UART_ID].txBuf[18]= (uint8_t)hSHT30.humidity;
	if (hRov.yt_angle<0)
	u16_buf = (uint16_t)(-(hRov.yt_angle)/10) | 0x0080;
	else
	u16_buf = (uint16_t)((hRov.yt_angle)/10);
	huart[HOST_UART_ID].txBuf[19]= u16_buf;
	huart[HOST_UART_ID].txBuf[20]= (uint8_t)((hRov.led_lumin1+hRov.led_lumin2)*50/LED_PWM_MAX);
	//huart[HOST_UART_ID].txBuf[21]= (uint8_t)(hRov.led_lumin2*100/LED_PWM_MAX);
	Turtle_Prop_PwrCuc();
	u16_buf = (uint16_t)((hRov.voltage/10)*(hRov.current/10));

	for (uint8_t i=0;i<8;i++)
	u32_dsp += hprop[i].Prop_RSp;
	if (u16_buf > 200 && u32_dsp < 4000)
	u16_buf = 0;

	huart[HOST_UART_ID].txBuf[22]= u16_buf>>8;
	huart[HOST_UART_ID].txBuf[23]= u16_buf;

	ShortConvertSign(hRov.pwr_temp,buf);
	huart[HOST_UART_ID].txBuf[24] = buf[0];
	huart[HOST_UART_ID].txBuf[25] = buf[1];

	for (uint8_t j=0;j<8;j++)
	{
		huart[HOST_UART_ID].txBuf[26+(j<<1)]		= hprop[j].Prop_RSp>>8;
		huart[HOST_UART_ID].txBuf[26+(j<<1)+1]	= hprop[j].Prop_RSp;
	}

	huart[HOST_UART_ID].txBuf[42] = host[1].fpid_check;
	for (uint8_t j=0;j<4;j++)
	{
		if ((host[1].fpid_check&hAuto[j].symbol) == hAuto[j].symbol)
		{
			huart[HOST_UART_ID].txBuf[43] = (uint8_t)hAuto[j].kp;
			huart[HOST_UART_ID].txBuf[44] = (uint8_t)((hAuto[j].kp-huart[HOST_UART_ID].txBuf[43])*100);
			huart[HOST_UART_ID].txBuf[45] = (uint8_t)hAuto[j].ki;
			huart[HOST_UART_ID].txBuf[46] = (uint8_t)((hAuto[j].ki-huart[HOST_UART_ID].txBuf[45])*100);
			huart[HOST_UART_ID].txBuf[47] = (uint8_t)hAuto[j].kd;
			huart[HOST_UART_ID].txBuf[48] = (uint8_t)((hAuto[j].kd-huart[HOST_UART_ID].txBuf[47])*100);
			if (hAuto[0].mode == MODE_CCPID)
			{
				switch(j)
				{
					case 0:
						huart[HOST_UART_ID].txBuf[43] = (uint8_t)hDeepAuto[0].kp;
						huart[HOST_UART_ID].txBuf[44] = (uint8_t)((hDeepAuto[0].kp-huart[HOST_UART_ID].txBuf[43])*100);
						huart[HOST_UART_ID].txBuf[45] = (uint8_t)hDeepAuto[0].ki;
						huart[HOST_UART_ID].txBuf[46] = (uint8_t)((hDeepAuto[0].ki-huart[HOST_UART_ID].txBuf[45])*100);
						huart[HOST_UART_ID].txBuf[47] = (uint8_t)hDeepAuto[0].kd;
						huart[HOST_UART_ID].txBuf[48] = (uint8_t)((hDeepAuto[0].kd-huart[HOST_UART_ID].txBuf[47])*100);
						break;
					case 3:
						huart[HOST_UART_ID].txBuf[43] = (uint8_t)hDeepAuto[1].kp;
						huart[HOST_UART_ID].txBuf[44] = (uint8_t)((hDeepAuto[1].kp-huart[HOST_UART_ID].txBuf[43])*100);
						huart[HOST_UART_ID].txBuf[45] = (uint8_t)hDeepAuto[1].ki;
						huart[HOST_UART_ID].txBuf[46] = (uint8_t)((hDeepAuto[1].ki-huart[HOST_UART_ID].txBuf[45 ])*100);
						huart[HOST_UART_ID].txBuf[47] = (uint8_t)hDeepAuto[1].kd;
						huart[HOST_UART_ID].txBuf[48] = (uint8_t)((hDeepAuto[1].kd-huart[HOST_UART_ID].txBuf[47])*100);
						break;
				}
			}
		}
	}
	
	//发送挡位
	for (uint8_t j=0;j<4;j++)
		if (gear_switch[j] == host[1].prop_gear)
			huart[HOST_UART_ID].txBuf[49] = j;
	
	//发送系统辨识相关设置
	huart[HOST_UART_ID].txBuf[50] = host[1].fsi_check;
	for (uint8_t j=0;j<4;j++)
	{
		if ((host[1].fsi_check&hSI[j].mode) == hSI[j].mode)
		{
			huart[HOST_UART_ID].txBuf[51] = hSI[j].period;
			huart[HOST_UART_ID].txBuf[52] = hSI[j].amp;
		}
	}

	for (uint8_t j=0;j<4;j++)
		huart[HOST_UART_ID].txBuf[53+j] = (uint8_t)((short)hSI[j].si_out + 0x80);

	u16_buf = CRC16_MODBUS(huart[HOST_UART_ID].txBuf,HOST_FLENGTH-4);
	huart[HOST_UART_ID].txBuf[60] = u16_buf;
	huart[HOST_UART_ID].txBuf[61] = u16_buf>>8;
	huart[HOST_UART_ID].txBuf[62] = 0x0D;
	huart[HOST_UART_ID].txBuf[63] = 0x0A;
	HAL_UART_Transmit_DMA(&HOST_UART,huart[HOST_UART_ID].txBuf,HOST_FLENGTH);
}

void Turtle_HostCtrlProp(void)
{
	for(uint8_t i=0;i<8;i++)
		hprop[i].Prop_CQ = host[1].joy_stick[i+KID_RB];
	
	host[1].auto_flag = 0x00;
	host[1].si_flag = 0x00;
}

void Turtle_HostData(uint8_t *data)
{
	uint8_t *p_buf;
	uint8_t *p_buf_back;
	uint8_t zero_cnt = 0;
	uint8_t i;
	
	/**************** 接收数据处理 ****************/
	if (data[0] == HOST_START_H && data[1] == HOST_START_L && data[2] == HOST_LENGTH)
	{
		if (data[HOST_LENGTH-2] == HOST_END_H && data[HOST_LENGTH-1] == HOST_END_L)
		{
			//键值及PID相关flag保存
			p_buf = host[1].joy_stick;
			for(i=0;i<20;i++)
			{
				p_buf[i] = data[3+i];
				if (p_buf[i] == 0)
					zero_cnt ++;
			}
			
			if (zero_cnt > 18)
			{
				host[1].joy_stick[KID_RSTICK_FB] = 0x80;
				host[1].joy_stick[KID_RSTICK_RL] = 0x80;
				host[1].joy_stick[KID_LSTICK_FB] = 0x80;
				host[1].joy_stick[KID_LSTICK_RL] = 0x80;
				host[1].joy_stick[KID_RT] = 0x80;
				host[1].joy_stick[KID_LT] = 0x80;
			}
			
			host[1].fpid_change	= data[23];
			host[1].fpid_check	= data[24];

			//自动控制参数确定
			for (i=0;i<4;i++)
			{	
				//PID参数修改
				if ((host[1].fpid_change&hAuto[i].symbol) == hAuto[i].symbol)
				{
					float pid_buf[3];
					pid_buf[0] = (float)data[25] + (float)data[26]*0.01;
					pid_buf[1] = (float)data[27] + (float)data[28]*0.01;
					pid_buf[2] = (float)data[29] + (float)data[30]*0.01;
					
					//串级PID参数处理
					if (hAuto[0].mode == MODE_CCPID)
					{
						switch(i)
						{
							case 0:
								hDeepAuto[0].kp = pid_buf[0];
								hDeepAuto[0].ki = pid_buf[1];
								hDeepAuto[0].kd = pid_buf[2];
								break;
							case 3:
								hDeepAuto[1].kp = pid_buf[0];
								hDeepAuto[1].ki = pid_buf[1];
								hDeepAuto[1].kd = pid_buf[2];
								break;
						}
					}
					else if (pid_buf[0] != hAuto[i].kp || pid_buf[1] != hAuto[i].ki || pid_buf[2] != hAuto[i].kd)
					{
						//uint16_t *pid_p = (uint16_t *)&data[25];
						hAuto[i].kp = pid_buf[0];
						hAuto[i].ki = pid_buf[1];
						hAuto[i].kd = pid_buf[2];
						//FLASH_Write(PID_START_ADD-i*8,pid_p,6);
					}
				}
				
				//自动控制
				if ((data[31]&hAuto[i].symbol) == hAuto[i].symbol)
				{
					hAuto[i].set_value = (float)(((uint16_t)data[32+2*i]<<8)+data[33+2*i]);
					host[1].auto_flag |= hAuto[i].symbol;
				}
				else if (hAuto[i].enable == AUTO_DISABLE)
				{
					host[1].auto_flag &= ~hAuto[i].symbol;
				}
			}
			
			//在上位机不使能自动控制的情况下，按键使能自动控制
			static uint8_t auto_key_cnt = 0;
			static uint8_t auto_key_flag = 0;
				
			if (data[31] == 0x00)
			{
				if (host[1].joy_stick[KID_X])
				{
					if (auto_key_cnt > 3)
					{
						hAuto[AUTO_HEADING_ID].set_value = hRov.yaw*10;
						hAuto[AUTO_HEADING_ID].enable = AUTO_ENABLE - hAuto[AUTO_HEADING_ID].enable;
						auto_key_flag ^= AUTO_ENABLE << AUTO_HEADING_ID;
					}
					auto_key_cnt = 0;
				}
				else if (host[1].joy_stick[KID_Y])
				{
					if (auto_key_cnt > 3)
					{
						hAuto[AUTO_DEPTH_ID].set_value = (float)hPress.conv_depth[0]/10;
						hAuto[AUTO_DEPTH_ID].enable = AUTO_ENABLE - hAuto[AUTO_DEPTH_ID].enable;
						auto_key_flag ^= AUTO_ENABLE << AUTO_DEPTH_ID;
					}
					auto_key_cnt = 0;
				}
				else
				{
					auto_key_cnt ++;
				}
				
				host[1].auto_flag = auto_key_flag;
			}
			
			//挡位读取
			//有效性验证
			//host[1].prop_gear = gear_switch[data[40]];
			
			//阈值和脉宽读取
			host[1].si_flag			= data[41];
			host[1].fsi_check		= data[42];
			host[1].fsi_change	= data[43];
			
			//系统辨识参数确定
			for (i=0;i<4;i++)
			{	
				//系统辨识参数修改
				if ((host[1].fsi_change&hSI[i].mode) == hSI[i].mode)
				{
					hSI[i].period = data[44];
					hSI[i].amp		= data[45];
				}
			}
			
			/**************** 数据处理 ****************/
			//键值处理
			for (i=0;i<4;i++)
			{
				//最小限制
				if (AbsMinus(host[1].joy_stick[i],KEY_MID) < KEY_MIN_GATE)
					host[1].joy_stick[i] = KEY_MID;
				if (host[1].prop_gear == 0)
					host[1].prop_gear = 1;
				host[1].joy_stick[i] = KeyValue_Div(host[1].joy_stick[i],host[1].prop_gear);
			}

#if DEPTH_GEAR_EN == 1u	
			host[1].joy_stick[KID_LT] = KeyValue_Div(host[1].joy_stick[KID_LT],host[1].prop_gear);
			host[1].joy_stick[KID_RT] = KeyValue_Div(host[1].joy_stick[KID_RT],host[1].prop_gear);
#endif
			
			//数据备份
			p_buf_back = host[0].joy_stick;
			for (i=0;i<sizeof(host[1]);i++)
				p_buf_back[i] = p_buf[i];
			
			//状态回复
			Turtle_HostFb();
		}
	}
}