/**
  ******************************************************************************
  * @file    turtle_movectrl.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 运动控制程序
  *
  ******************************************************************************
  * @attention
  *	运动方向说明(水平推进器如下所示，垂向推进器向下为正)：
	*		Ⅳ↗*******↖Ⅰ
	*				*			*
	*				 *   *
	*          *				
	*        *   *
	*       *     *
  *		Ⅲ↖*******↗Ⅱ
	*
	*	关于摇杆的说明:
	*		摇杆向上为0; 向左为0.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

/*关于正方向的定义:前进为正、后退为负
									顺时针为正、逆时针为负
									面向前进的方向，向右为正、向左为负
									向下为正，向上为负
*/

uint16_t  prop_buf;
int16_t   key_buf;

float			prop_inter = (float)(PROP_CQ_MAX - PROP_CQ_MIN)/250;

static short Turtle_Prop_CovValue(uint8_t key_val)
{
	key_val>252?key_val=252:(key_val < 2?key_val = 2:key_val);
	return (short)(((float)key_val-0x80)*prop_inter);
}

//推进器上浮下潜
void Turtle_Prop_UpDown(uint8_t key)
{
	uint8_t i = PROPV_RF;
	key_buf = Turtle_Prop_CovValue(key);
	prop_buf = PROP_CQ_MID + (int16_t)((float)key_buf*DEPTH_K);
	
	for(;i<=PROP_ID8;i++)
		hprop[i].Prop_CQ = prop_buf;
}

//推进器前进后退
void Turtle_Prop_FwBack(uint8_t key)
{
	uint8_t i = PROPL_RF;
	prop_buf = PROP_CQ_MID + Turtle_Prop_CovValue(key);
	for(;i<=PROP_ID4;i++)
		hprop[i].Prop_CQ = prop_buf;
}

float side_k = 1;
//推进器横移
void Turtle_Prop_Side(uint8_t key)
{
	key_buf = Turtle_Prop_CovValue(key);
	//1,3反向
	prop_buf = PROP_CQ_MID - key_buf;
	hprop[PROP_ID1].Prop_CQ = prop_buf;
	hprop[PROP_ID3].Prop_CQ = prop_buf;
	
	//2,4正向
	prop_buf = PROP_CQ_MID + (uint16_t)((float)key_buf*side_k);
	hprop[PROP_ID2].Prop_CQ = prop_buf;
	hprop[PROP_ID4].Prop_CQ = prop_buf;
}
//推进器前向转弯
void Turtle_Prop_FHeading(uint8_t key)
{
	uint16_t buf[2];
	//前进方向 4加 1减
	buf[0] = hprop[PROP_ID1].Prop_CQ;
	buf[1] = hprop[PROP_ID4].Prop_CQ;
	
	key_buf = Turtle_Prop_CovValue(key);
	
	buf[1] += key_buf, buf[0] -= key_buf;
	
	buf[0]>PROP_CQ_MAX?buf[0]=PROP_CQ_MAX:(buf[0]<PROP_CQ_MIN?buf[0]=PROP_CQ_MIN:buf[0]);
	buf[1]>PROP_CQ_MAX?buf[1]=PROP_CQ_MAX:(buf[1]<PROP_CQ_MIN?buf[1]=PROP_CQ_MIN:buf[1]);
	
	hprop[PROP_ID1].Prop_CQ = buf[0];
	hprop[PROP_ID4].Prop_CQ = buf[1];
}
	
//推进器后退转弯
void Turtle_Prop_BHeading(uint8_t key)
{
	uint16_t buf[2];
	//后退方向 2加 3减
	buf[0] = hprop[PROP_ID2].Prop_CQ;
	buf[1] = hprop[PROP_ID3].Prop_CQ;
	
	key_buf = Turtle_Prop_CovValue(key);
	
	buf[0] += key_buf, buf[1] -= key_buf;
	
	buf[0]>PROP_CQ_MAX?buf[0]=PROP_CQ_MAX:(buf[0]<PROP_CQ_MIN?buf[0]=PROP_CQ_MIN:buf[0]);
	buf[1]>PROP_CQ_MAX?buf[1]=PROP_CQ_MAX:(buf[1]<PROP_CQ_MIN?buf[1]=PROP_CQ_MIN:buf[1]);
	
	hprop[PROP_ID2].Prop_CQ = buf[0];
	hprop[PROP_ID3].Prop_CQ = buf[1];
}

//推进器转弯
void Turtle_Prop_Heading(uint8_t key)
{
	//首先判断ROV运动方向
	if ((hRov.run_state & ROV_RUN_BACK) == ROV_RUN_BACK)
	{
		Turtle_Prop_BHeading(key);
	}
	else
	{
		Turtle_Prop_FHeading(key);
	}

}

//推进器自转
void Turtle_Prop_Rotation(uint8_t key)
{
	key_buf = Turtle_Prop_CovValue(key);
	//1、2反向
	hprop[PROP_ID1].Prop_CQ = PROP_CQ_MID - key_buf;
	hprop[PROP_ID2].Prop_CQ = PROP_CQ_MID - key_buf;
	
	//3、4正向
	hprop[PROP_ID3].Prop_CQ = PROP_CQ_MID + key_buf;
	hprop[PROP_ID4].Prop_CQ = PROP_CQ_MID + key_buf;
}

//推进器俯仰（正方向向上）
void Turtle_Prop_Pitch(uint8_t key)
{
	key_buf = Turtle_Prop_CovValue(key);
	//5、8反向
	hprop[PROP_ID5].Prop_CQ = PROP_CQ_MID - key_buf;
	hprop[PROP_ID8].Prop_CQ = PROP_CQ_MID - key_buf;
	
	//6、7正向
	hprop[PROP_ID6].Prop_CQ = PROP_CQ_MID + key_buf;
	hprop[PROP_ID7].Prop_CQ = PROP_CQ_MID + key_buf;
}

//推进器翻转（正方向逆时针）
void Turtle_Prop_Roll(uint8_t key)
{
	key_buf = Turtle_Prop_CovValue(key);
	//5、6反向
	hprop[PROP_ID5].Prop_CQ = PROP_CQ_MID - key_buf;
	hprop[PROP_ID6].Prop_CQ = PROP_CQ_MID - key_buf;
	
	//7、8正向向
	hprop[PROP_ID7].Prop_CQ = PROP_CQ_MID + key_buf;
	hprop[PROP_ID8].Prop_CQ = PROP_CQ_MID + key_buf;
}

//推进器姿态控制(叠加)
void Turtle_Prop_AtPlus(uint8_t *key)
{
	uint16_t buf;
	for (uint8_t i=0;i<4;i++)
	{
		buf = hprop[4+i].Prop_CQ;
		key_buf = Turtle_Prop_CovValue(key[i]);
		
		buf += key_buf;
		buf>PROP_CQ_MAX?buf=PROP_CQ_MAX:(buf<PROP_CQ_MIN?buf=PROP_CQ_MIN:buf);
		hprop[4+i].Prop_CQ = buf;
	}
}

//推进器姿态控制(分配)
void Turtle_Prop_AtDrt(uint8_t *key)
{
	for (uint8_t i=0;i<4;i++)
	{
		key_buf = Turtle_Prop_CovValue(key[i]);
		hprop[4+i].Prop_CQ = PROP_CQ_MID + key_buf;
	}
}

//水平推进器清零
void Turtle_Prop_PCLC(void)
{
	for(uint8_t i=0;i<4;i++)
		hprop[PROP_ID1+i].Prop_CQ = PROP_CQ_MID;
}
	
//垂向推进器清零
void Turtle_Prop_VCLC(void)
{
	for(uint8_t i=0;i<4;i++)
		hprop[PROP_ID5+i].Prop_CQ = PROP_CQ_MID;
}