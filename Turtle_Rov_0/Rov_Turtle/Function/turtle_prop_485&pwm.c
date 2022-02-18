/**
  ******************************************************************************
  * @file    turtle_prop_485&pwm.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 推进器控制程序
  *
  ******************************************************************************
  * @attention
  *	同时包含PWM及485控制
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

Prop_CtrlTypeDef hprop[8];

static uint8_t prop_ecnt[8] = {0};

//static TIM_HandleTypeDef *tim_handle[] = {&PROP1_4_TIM,&PROP1_4_TIM,&PROP1_4_TIM,&PROP1_4_TIM,&PROP5_8_TIM,&PROP5_8_TIM,&PROP5_8_TIM,&PROP5_8_TIM};
//static uint32_t						tim_ch[]     = {PROP_CH1,PROP_CH2,PROP_CH3,PROP_CH4,PROP_CH5,PROP_CH6,PROP_CH7,PROP_CH8};

static uint16_t AbsMinus(uint16_t arg1, uint16_t arg2)
{
	if (arg1>arg2)
		return arg1-arg2;
	else
		return arg2-arg1;
}

void Turtle_Prop_Init(void)
{
	uint8_t i = 0;
	for(i=0;i<8;i++)
	{
		hprop[i].Prop_CQ = 1500;
		hprop[i].Prop_OCQ = 1500;
		hprop[i].Prop_Gear = 1;
		hprop[i].Prop_ID = i+1;
	}
}

void Turtle_Prop_Update(void)
{
	uint16_t crc16_buf;
	HAL_StatusTypeDef ret;
	
	static uint8_t prop_cnt = 0;
	
	if ((hprop[prop_cnt].Prop_State & PROP_FBD)  ==  PROP_FBD)
		hprop[prop_cnt].Prop_CQ = PROP_CQ_MID;
	
	if (hprop[prop_cnt].Prop_CQ < PROP_CQ_MAX+10 && hprop[prop_cnt].Prop_CQ > PROP_CQ_MIN-10)
	{
		huart[PROP_UART_ID].txBuf[0] = hprop[prop_cnt].Prop_ID;
		huart[PROP_UART_ID].txBuf[1] = 0x06;
		huart[PROP_UART_ID].txBuf[2] = 0x86;
		huart[PROP_UART_ID].txBuf[3] = hprop[prop_cnt].Prop_CQ>>8;
		huart[PROP_UART_ID].txBuf[4] = hprop[prop_cnt].Prop_CQ;
		crc16_buf = CRC16_MODBUS(huart[PROP_UART_ID].txBuf,5);
		huart[PROP_UART_ID].txBuf[5] = crc16_buf;
		huart[PROP_UART_ID].txBuf[6] = crc16_buf>>8;
		huart[PROP_UART_ID].txBufCount = 7;
		HAL_GPIO_WritePin(P485_EN_GPIO_Port,P485_EN_Pin,GPIO_PIN_SET);
		ret = HAL_UART_Transmit_DMA(&PROP_UART,huart[PROP_UART_ID].txBuf,huart[PROP_UART_ID].txBufCount);
		hprop[prop_cnt].Prop_State |= PROP_BUSY,hprop[prop_cnt].Prop_State &= ~PROP_BUSY;
	}
	
	if ((hprop[prop_cnt].Prop_State & (PROP_READY | PROP_FBD | PROP_ERR)) !=  0x00)
		prop_cnt++;
	else
	{
		if (prop_ecnt[prop_cnt] <= PROP_ERR_MAX)
			prop_ecnt[prop_cnt] ++;
		else
		{
			hprop[prop_cnt].Prop_State |= PROP_ERR;
			prop_ecnt[prop_cnt] = 0;
		}
	}
	
	//水平和垂向分开控制，减少电源冲击的情况
	if (prop_cnt == 3)
	{
		uint8_t err_cnt = 0;
		for (uint8_t i=0;i<4;i++)
			if (hprop[prop_cnt].Prop_CQ > PROP_CQ_MAX+10 && hprop[prop_cnt].Prop_CQ < PROP_CQ_MIN-10)
				err_cnt ++;
			
		if (err_cnt == 0)
		{
			__HAL_TIM_SET_COMPARE(&PROP1_4_TIM,PROP_CH1,hprop[0].Prop_CQ);
			__HAL_TIM_SET_COMPARE(&PROP1_4_TIM,PROP_CH2,hprop[1].Prop_CQ);
			__HAL_TIM_SET_COMPARE(&PROP1_4_TIM,PROP_CH3,hprop[2].Prop_CQ);
			__HAL_TIM_SET_COMPARE(&PROP1_4_TIM,PROP_CH4,hprop[3].Prop_CQ);
		}
	}
	
	if (prop_cnt == 7)
	{
		uint8_t err_cnt = 0;
		prop_cnt = 0;
		for (uint8_t i=4;i<8;i++)
			if (hprop[prop_cnt].Prop_CQ > PROP_CQ_MAX+10 && hprop[prop_cnt].Prop_CQ < PROP_CQ_MIN-10)
				err_cnt ++;
			
		if (err_cnt == 0)
		{
			__HAL_TIM_SET_COMPARE(&PROP5_8_TIM,PROP_CH5,hprop[4].Prop_CQ);
			__HAL_TIM_SET_COMPARE(&PROP5_8_TIM,PROP_CH6,hprop[5].Prop_CQ);
			__HAL_TIM_SET_COMPARE(&PROP5_8_TIM,PROP_CH7,hprop[6].Prop_CQ);
			__HAL_TIM_SET_COMPARE(&PROP5_8_TIM,PROP_CH8,hprop[7].Prop_CQ);
			
#if PROP_MODE == PROP_PWM
			static float k_prop_pwm = (float)PROP_RSP_MAX/(PROP_CQ_MAX - PROP_CQ_MID);
			for (uint8_t i=0;i<8;i++)
			{
				hprop[i].Prop_DRSp = (int16_t)((float)((int16_t)hprop[i].Prop_CQ - PROP_CQ_MID)*k_prop_pwm);
				
				if (hprop[i].Prop_DRSp < 0)
					hprop[i].Prop_RSp = -hprop[i].Prop_DRSp;
				else
					hprop[i].Prop_RSp = hprop[i].Prop_DRSp;
				
				if (hprop[i].Prop_CQ < PROP_CQ_MID+PROP_CQ_GATE && hprop[i].Prop_CQ > PROP_CQ_MID-PROP_CQ_GATE && hprop[i].Prop_RSp < PROP_RSP_GATE)
					hprop[i].Prop_RSp = 0; 
			}
			for (uint8_t i=0;i<8;i++)
				hprop[i].Prop_State |= PROP_READY;
#endif
		}
	}
}

void Turtle_Prop_DataPro(uint8_t *prop_rt)
{
	//返回数据处理
	uint8_t i;

#if PROP_MODE == PROP_485
	for (i=0;i<8;i++)
	{
		if(hprop[i].Prop_ID == prop_rt[0])
		{
			if (prop_rt[1] == 0x06 && prop_rt[2] == 0x86)
			{
				hprop[i].Prop_Vot = prop_rt[3];
				hprop[i].Prop_Cur = prop_rt[4];
				hprop[i].Prop_RSp = ((uint16_t)prop_rt[5]<<8)+prop_rt[6];
				if (hprop[i].Prop_CQ < PROP_CQ_MID+PROP_CQ_GATE && hprop[i].Prop_CQ > PROP_CQ_MID-PROP_CQ_GATE && hprop[i].Prop_RSp < PROP_RSP_GATE)
					hprop[i].Prop_RSp = 0; 
				else if (hprop[i].Prop_RSp > PROP_RSP_GMAX)
					hprop[i].Prop_RSp = PROP_RSP_GMAX;
				
				if (hprop[i].Prop_CQ < PROP_CQ_MID)
				{
					hprop[i].Prop_DRSp = -hprop[i].Prop_RSp;
				}
				else
					hprop[i].Prop_DRSp = hprop[i].Prop_RSp;
				hprop[i].Prop_State |= PROP_READY, hprop[i].Prop_State &= ~PROP_ERR;
			}
			break;
		}
	}
#endif
}

void Turtle_Prop_SetGear(uint8_t gear)
{
	for (uint8_t i=0;i<8;i++)
	{
		hprop[i].Prop_Gear = gear;
	}
}

void Turtle_Prop_PwrCuc(void)
{
	uint16_t volt_buf = 0;
	uint16_t cur_buf = 0;
	for (uint8_t i=0;i<8;i++)
	{
		volt_buf	+= hprop[i].Prop_Vot;
		cur_buf		+= hprop[i].Prop_Cur;
	}
	
	hRov.voltage = volt_buf/8;
	hRov.current = cur_buf;
}


#define LR_GATE	100
#define CTRL_K	1

//推进器运行状态检测
void Turtle_Prop_RunCheck(void)
{
	uint8_t i;
	uint32_t buf = 0x800000;
	static uint8_t p_cnt = 0;
	static uint8_t v_cntp = 0;
	static uint8_t v_cntm = 0;
	
	//推进器异常检测
	for (i=0;i<8;i++)
	{
		if (hprop[i].Prop_RSp < PROP_RSP_GMIN && AbsMinus(hprop[i].Prop_CQ,PROP_CQ_MID) > PROP_CQ_GATE)
			prop_ecnt[i] ++;
		else if (hprop[i].Prop_RSp > PROP_RSP_GATE && AbsMinus(hprop[i].Prop_CQ,PROP_CQ_MID) < PROP_CQ_GATE)
			prop_ecnt[i] ++;
		else
			hprop[i].Prop_State &= ~PROP_ERR;
		
		if (hprop[i].Prop_RSp < AbsMinus(hprop[i].Prop_CQ,PROP_CQ_MID)*CTRL_K && AbsMinus(hprop[i].Prop_CQ,PROP_CQ_MID) > PROP_CQ_GATE)
			hprop[i].Prop_State |= PROP_RERR;
		
		if ((hprop[i].Prop_State & PROP_ERR) == PROP_ERR)
			hRov.run_state |= buf>>i;
		else
			hRov.run_state &= ~(buf>>i);
	}
	
	//水平运动判断
	p_cnt = 0;
	for (i=0;i<4;i++)
		if (hprop[i].Prop_RSp < PROP_RSP_GMIN)
			p_cnt ++;
	
	if (p_cnt > 2)
	{
		hRov.run_state &= ~(ROV_TURN_LEFT|ROV_TURN_RIGHT|ROV_RUN_FORWARD|ROV_RUN_BACK);
	}
	else
	{
		//判断水平运行方向
		if (hprop[PROP_ID2].Prop_CQ>PROP_CQ_MID && hprop[PROP_ID3].Prop_CQ>PROP_CQ_MID)
		{
			hRov.run_state |= ROV_RUN_FORWARD,hRov.run_state &= ~ROV_RUN_BACK;
			//判断水平转弯方向
#if G_PROP == G_PROP_N
			if ((hprop[PROP_ID4].Prop_DRSp - hprop[PROP_ID1].Prop_DRSp) > LR_GATE)
				hRov.run_state |= ROV_TURN_LEFT,hRov.run_state &= ~ROV_TURN_RIGHT;
			else if ((hprop[PROP_ID1].Prop_DRSp - hprop[PROP_ID4].Prop_DRSp) > LR_GATE)
				hRov.run_state |= ROV_TURN_RIGHT,hRov.run_state &= ~ROV_TURN_LEFT;
			else
				hRov.run_state &= ~ROV_TURN_LEFT,hRov.run_state &= ~ROV_TURN_RIGHT;
#else
			if ((hprop[PROP_ID4].Prop_DRSp - hprop[PROP_ID1].Prop_DRSp) > LR_GATE)
				hRov.run_state |= ROV_TURN_RIGHT,hRov.run_state &= ~ROV_TURN_LEFT;
			else if ((hprop[PROP_ID1].Prop_DRSp - hprop[PROP_ID4].Prop_DRSp) > LR_GATE)
				hRov.run_state |= ROV_TURN_LEFT,hRov.run_state &= ~ROV_TURN_RIGHT;
			else
				hRov.run_state &= ~ROV_TURN_LEFT,hRov.run_state &= ~ROV_TURN_RIGHT;
#endif
		}
#if G_PROP == G_PROP_N
		else if (hprop[PROP_ID1].Prop_CQ>PROP_CQ_MID && hprop[PROP_ID4].Prop_CQ>PROP_CQ_MID)
#else
		else if (hprop[PROP_ID1].Prop_CQ<PROP_CQ_MID && hprop[PROP_ID4].Prop_CQ<PROP_CQ_MID)
#endif
		{
			hRov.run_state |= ROV_RUN_BACK,hRov.run_state &= ~ROV_RUN_FORWARD;
			//判断水平转弯方向
			if (hprop[PROP_ID2].Prop_DRSp - hprop[PROP_ID3].Prop_DRSp > LR_GATE)
				hRov.run_state |= ROV_TURN_RIGHT,hRov.run_state &= ~ROV_TURN_LEFT;
			else if (hprop[PROP_ID3].Prop_DRSp - hprop[PROP_ID2].Prop_DRSp > LR_GATE)
				hRov.run_state |= ROV_TURN_LEFT,hRov.run_state &= ~ROV_TURN_RIGHT;
			else
				hRov.run_state &= ~ROV_TURN_LEFT,hRov.run_state &= ~ROV_TURN_LEFT;
		}
		else
		{
			hRov.run_state &= ~ROV_RUN_FORWARD,hRov.run_state &= ~ROV_RUN_BACK;
			//判断原地转弯方向
			if ((hprop[PROP_ID4].Prop_DRSp - hprop[PROP_ID1].Prop_DRSp) > LR_GATE)
				hRov.run_state |= ROV_TURN_RIGHT,hRov.run_state &= ~ROV_TURN_LEFT;
			else if ((hprop[PROP_ID1].Prop_DRSp - hprop[PROP_ID4].Prop_DRSp) > LR_GATE)
				hRov.run_state |= ROV_TURN_LEFT,hRov.run_state &= ~ROV_TURN_RIGHT;
			else
				hRov.run_state &= ~ROV_TURN_LEFT,hRov.run_state &= ~ROV_TURN_RIGHT;;
		}
	}

	
	//垂向运动判断
	v_cntp = 0;
	for (i=4;i<8;i++)
		if (hprop[i].Prop_RSp < PROP_RSP_GMIN)
			v_cntp ++;
		
	if (v_cntp > 2)
	{
		hRov.run_state &= ~(ROV_RUN_UP|ROV_RUN_DOWN);
	}
	else
	{
		//判断垂向运动方向
		v_cntp = 0;
		v_cntm = 0;
		for (i=4;i<8;i++)
			if (hprop[i].Prop_DRSp > 0)
				v_cntp ++;
			else if (hprop[i].Prop_DRSp < 0)
				v_cntm ++;
			
		if (v_cntp > 2)
			hRov.run_state |= ROV_RUN_DOWN,hRov.run_state &= ~ROV_RUN_UP;
		else
			hRov.run_state |= ROV_RUN_UP,hRov.run_state &= ~ROV_RUN_DOWN;
	}
}

//推进器异常处理
void Turtle_Prop_ErrPro(void)
{
	uint8_t i = 0;
	
	//水平推进器异常处理
	for (i=0;i<4;i++)
	{
		if ((hprop[i].Prop_State & PROP_ERR)  != 0x00)
			hprop[3-i].Prop_State |= PROP_FBD;
		else
			hprop[3-i].Prop_State &= ~PROP_FBD;
	}
	
	//垂向推进器异常处理
	for (i=4;i<8;i++)
	{
		switch(i)
		{
			case 4:
			case 6:
				if ((hprop[i].Prop_State & PROP_ERR)  != 0x00)
					hprop[10-i].Prop_State |= PROP_FBD;
				else
					hprop[10-i].Prop_State &= ~PROP_FBD;
			break;
			
			case 5:
			case 7:
				if ((hprop[i].Prop_State & PROP_ERR)  != 0x00)
					hprop[12-i].Prop_State |= PROP_FBD;
				else
					hprop[12-i].Prop_State &= ~PROP_FBD;
			break;
		}
	}
}

//推进器阈值处理及逻辑处理
uint16_t cq_sum_max = (uint16_t)((float)(PROP_CQ_MAX-PROP_CQ_MID)*CQ_SUM_MAX*8);

void Turtle_Prop_GatePro(void)
{
	uint16_t cq_sum = 0;
			
#if G_PROP == G_PROP_N
	hprop[PROP_ID1].Prop_CQ = 2*PROP_CQ_MID - hprop[PROP_ID1].Prop_CQ;
	hprop[PROP_ID4].Prop_CQ = 2*PROP_CQ_MID - hprop[PROP_ID4].Prop_CQ;
#endif
	
	for(uint8_t i=0;i<8;i++)
		cq_sum += AbsMinus(hprop[i].Prop_CQ,PROP_CQ_MID);
	
	for(uint8_t i=0;i<8;i++)
	{
		if (cq_sum > cq_sum_max)
			hprop[i].Prop_CQ = hprop[i].Prop_OCQ;
		else if (hprop[i].Prop_CQ - hprop[i].Prop_OCQ > hRov.prop_cq_gmax)
			hprop[i].Prop_CQ = hprop[i].Prop_OCQ + hRov.prop_cq_gmax;
		else if (hprop[i].Prop_CQ - hprop[i].Prop_OCQ < -hRov.prop_cq_gmax)
			hprop[i].Prop_CQ = hprop[i].Prop_OCQ - hRov.prop_cq_gmax;
		
		hprop[i].Prop_OCQ = hprop[i].Prop_CQ;
	}
}
