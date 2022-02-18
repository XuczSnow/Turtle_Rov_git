/**
  ******************************************************************************
  * @file    turtle_movectrl.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle ϵ�� ROV �˶����Ƴ���
  *
  ******************************************************************************
  * @attention
  *	�˶�����˵��(ˮƽ�ƽ���������ʾ�������ƽ�������Ϊ��)��
	*		���J*******�I��
	*				*			*
	*				 *   *
	*          *				
	*        *   *
	*       *     *
  *		��I*******�J��
	*
	*	����ҡ�˵�˵��:
	*		ҡ������Ϊ0; ����Ϊ0.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

/*����������Ķ���:ǰ��Ϊ��������Ϊ��
									˳ʱ��Ϊ������ʱ��Ϊ��
									����ǰ���ķ�������Ϊ��������Ϊ��
									����Ϊ��������Ϊ��
*/

uint16_t  prop_buf;
int16_t   key_buf;

float			prop_inter = (float)(PROP_CQ_MAX - PROP_CQ_MIN)/250;

static short Turtle_Prop_CovValue(uint8_t key_val)
{
	key_val>252?key_val=252:(key_val < 2?key_val = 2:key_val);
	return (short)(((float)key_val-0x80)*prop_inter);
}

//�ƽ����ϸ���Ǳ
void Turtle_Prop_UpDown(uint8_t key)
{
	uint8_t i = PROPV_RF;
	key_buf = Turtle_Prop_CovValue(key);
	prop_buf = PROP_CQ_MID + (int16_t)((float)key_buf*DEPTH_K);
	
	for(;i<=PROP_ID8;i++)
		hprop[i].Prop_CQ = prop_buf;
}

//�ƽ���ǰ������
void Turtle_Prop_FwBack(uint8_t key)
{
	uint8_t i = PROPL_RF;
	prop_buf = PROP_CQ_MID + Turtle_Prop_CovValue(key);
	for(;i<=PROP_ID4;i++)
		hprop[i].Prop_CQ = prop_buf;
}

float side_k = 1;
//�ƽ�������
void Turtle_Prop_Side(uint8_t key)
{
	key_buf = Turtle_Prop_CovValue(key);
	//1,3����
	prop_buf = PROP_CQ_MID - key_buf;
	hprop[PROP_ID1].Prop_CQ = prop_buf;
	hprop[PROP_ID3].Prop_CQ = prop_buf;
	
	//2,4����
	prop_buf = PROP_CQ_MID + (uint16_t)((float)key_buf*side_k);
	hprop[PROP_ID2].Prop_CQ = prop_buf;
	hprop[PROP_ID4].Prop_CQ = prop_buf;
}
//�ƽ���ǰ��ת��
void Turtle_Prop_FHeading(uint8_t key)
{
	uint16_t buf[2];
	//ǰ������ 4�� 1��
	buf[0] = hprop[PROP_ID1].Prop_CQ;
	buf[1] = hprop[PROP_ID4].Prop_CQ;
	
	key_buf = Turtle_Prop_CovValue(key);
	
	buf[1] += key_buf, buf[0] -= key_buf;
	
	buf[0]>PROP_CQ_MAX?buf[0]=PROP_CQ_MAX:(buf[0]<PROP_CQ_MIN?buf[0]=PROP_CQ_MIN:buf[0]);
	buf[1]>PROP_CQ_MAX?buf[1]=PROP_CQ_MAX:(buf[1]<PROP_CQ_MIN?buf[1]=PROP_CQ_MIN:buf[1]);
	
	hprop[PROP_ID1].Prop_CQ = buf[0];
	hprop[PROP_ID4].Prop_CQ = buf[1];
}
	
//�ƽ�������ת��
void Turtle_Prop_BHeading(uint8_t key)
{
	uint16_t buf[2];
	//���˷��� 2�� 3��
	buf[0] = hprop[PROP_ID2].Prop_CQ;
	buf[1] = hprop[PROP_ID3].Prop_CQ;
	
	key_buf = Turtle_Prop_CovValue(key);
	
	buf[0] += key_buf, buf[1] -= key_buf;
	
	buf[0]>PROP_CQ_MAX?buf[0]=PROP_CQ_MAX:(buf[0]<PROP_CQ_MIN?buf[0]=PROP_CQ_MIN:buf[0]);
	buf[1]>PROP_CQ_MAX?buf[1]=PROP_CQ_MAX:(buf[1]<PROP_CQ_MIN?buf[1]=PROP_CQ_MIN:buf[1]);
	
	hprop[PROP_ID2].Prop_CQ = buf[0];
	hprop[PROP_ID3].Prop_CQ = buf[1];
}

//�ƽ���ת��
void Turtle_Prop_Heading(uint8_t key)
{
	//�����ж�ROV�˶�����
	if ((hRov.run_state & ROV_RUN_BACK) == ROV_RUN_BACK)
	{
		Turtle_Prop_BHeading(key);
	}
	else
	{
		Turtle_Prop_FHeading(key);
	}

}

//�ƽ�����ת
void Turtle_Prop_Rotation(uint8_t key)
{
	key_buf = Turtle_Prop_CovValue(key);
	//1��2����
	hprop[PROP_ID1].Prop_CQ = PROP_CQ_MID - key_buf;
	hprop[PROP_ID2].Prop_CQ = PROP_CQ_MID - key_buf;
	
	//3��4����
	hprop[PROP_ID3].Prop_CQ = PROP_CQ_MID + key_buf;
	hprop[PROP_ID4].Prop_CQ = PROP_CQ_MID + key_buf;
}

//�ƽ������������������ϣ�
void Turtle_Prop_Pitch(uint8_t key)
{
	key_buf = Turtle_Prop_CovValue(key);
	//5��8����
	hprop[PROP_ID5].Prop_CQ = PROP_CQ_MID - key_buf;
	hprop[PROP_ID8].Prop_CQ = PROP_CQ_MID - key_buf;
	
	//6��7����
	hprop[PROP_ID6].Prop_CQ = PROP_CQ_MID + key_buf;
	hprop[PROP_ID7].Prop_CQ = PROP_CQ_MID + key_buf;
}

//�ƽ�����ת����������ʱ�룩
void Turtle_Prop_Roll(uint8_t key)
{
	key_buf = Turtle_Prop_CovValue(key);
	//5��6����
	hprop[PROP_ID5].Prop_CQ = PROP_CQ_MID - key_buf;
	hprop[PROP_ID6].Prop_CQ = PROP_CQ_MID - key_buf;
	
	//7��8������
	hprop[PROP_ID7].Prop_CQ = PROP_CQ_MID + key_buf;
	hprop[PROP_ID8].Prop_CQ = PROP_CQ_MID + key_buf;
}

//�ƽ�����̬����(����)
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

//�ƽ�����̬����(����)
void Turtle_Prop_AtDrt(uint8_t *key)
{
	for (uint8_t i=0;i<4;i++)
	{
		key_buf = Turtle_Prop_CovValue(key[i]);
		hprop[4+i].Prop_CQ = PROP_CQ_MID + key_buf;
	}
}

//ˮƽ�ƽ�������
void Turtle_Prop_PCLC(void)
{
	for(uint8_t i=0;i<4;i++)
		hprop[PROP_ID1+i].Prop_CQ = PROP_CQ_MID;
}
	
//�����ƽ�������
void Turtle_Prop_VCLC(void)
{
	for(uint8_t i=0;i<4;i++)
		hprop[PROP_ID5+i].Prop_CQ = PROP_CQ_MID;
}