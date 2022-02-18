/**
  ******************************************************************************
  * @file    turtle_autoctrl.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 自动控制程序
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

#include "de.h"
#include "math.h"

#define CYCLE				0.2f
#define CTRL_MODE		MODE_PID

AutoCtrl_StateTypeDef hAuto[4];
AutoCtrl_StateTypeDef hDeepAuto[2];

float u[3] 		=  {0};			//控制量缓存

uint8_t de_en = 0;
uint8_t out_sqrt_gate = 1;

float pid_kp[] = {6.0,	0.1,	0.4,	0.0};
float pid_ki[] = {0.3,	0.4,	0.2,	0.0};
float pid_kd[] = {2.5,	0.4,	0.4,	0.0};
float pid_db[] = {  0,    0,    0,    0};

float deep_kp[] = {};
float deep_ki[] = {};
float deep_kd[] = {};

AutoCtrl_ModeTypeDef mode[4] = {MODE_PID,MODE_SGPID,MODE_PID,MODE_PID};

void Turtle_AutoCtrl_Init(void)
{
	uint8_t i;
	uint8_t pid_buf[6];

	for(i=0;i<4;i++)
	{
		//参数读取
//		FLASH_Read(PID_START_ADD-i*8,(uint16_t *)pid_buf,6);
//		if (pid_buf[0] != 0xFF)
//		{
//			pid_kp[i] = (float)pid_buf[0] + (float)pid_buf[1]*0.01;
//			pid_ki[i] = (float)pid_buf[2] + (float)pid_buf[3]*0.01;
//			pid_kd[i] = (float)pid_buf[4] + (float)pid_buf[5]*0.01;
//		}
		//参数初始化
		hAuto[i].act_value	= 0;
		hAuto[i].set_value	= 0;
		hAuto[i].enable			= AUTO_DISABLE;
		hAuto[i].kp					= pid_kp[i];
		hAuto[i].ki					= pid_ki[i];
		hAuto[i].kd					= pid_kd[i];
		hAuto[i].out_db			= pid_db[i];
		hAuto[i].mode				= mode[i];
		hAuto[i].symbol			= 1<<i;
		HAL_Delay(10);
	}
	
	//深度输出补偿
	hAuto[0].out_dc = 36;
	hAuto[0].out_mode = OUT_DC;
}

static float rov_pow(float buf, uint8_t cnt)
{
	uint8_t i;
	for (i=0;i<cnt;i++)
		buf *= buf;
	return buf;
}

static float out_max = 125;
static float out_mid = 128;
static float out_gate= 1;

static void PID_OutOrg(AutoCtrl_StateTypeDef *hPid)
{
	float u_buf;
	u_buf = hPid->pidout;
	if (u_buf > out_max)
		u_buf = out_max;
	else if(u_buf < out_gate && u_buf > -out_gate)
		u_buf = 0;
	else if (u_buf < -out_max)
		u_buf = -out_max;
	
	hPid->pidout = u_buf;
}

static void Depth_OutDc(AutoCtrl_StateTypeDef *hPid)
{
	float u_buf;
	
	hPid->pidout += hPid->out_dc;
	if (u_buf > out_max)
		u_buf = out_max;
	else if (u_buf < 0)
	{
		u_buf /=0.7;
		if (u_buf < -91)
			u_buf = -91;
	}
	
	hPid->pidout = u_buf;
}

/*PID控制参数刷新
  1.刷新实际值
*/
void Turtle_AutoCtrl_Refresh(void)
{
	//刷新PID实际值
	hAuto[AUTO_DEPTH_ID].act_value		= (float)hPress.conv_depth[0]*0.1;
	hAuto[AUTO_HEADING_ID].act_value	= hRov.yaw*10;
	hAuto[AUTO_PITCH_ID].act_value		= hRov.pitch*10;
	hAuto[AUTO_ROLL_ID].act_value			= hRov.roll*10;
	
	//刷新串级PID实际值
	hDeepAuto[0].act_value = (float)hPress.conv_depth[0]*0.1;
	hDeepAuto[1].act_value = hPress.conv_speed[0];
}

static void Turtle_Auto_CCPID(void)
{
	hDeepAuto[0].enable = AUTO_ENABLE;
	hDeepAuto[1].enable = AUTO_ENABLE;
	//外环PID计算
	hDeepAuto[0].set_value = hAuto[0].set_value;
	Turtle_AutoCtrl(&hDeepAuto[0]);
	
	//内环PID计算
	hDeepAuto[1].set_value = hDeepAuto[0].pidout;
	Turtle_AutoCtrl(&hDeepAuto[1]);
	
	hAuto[0].pidout = hDeepAuto[1].pidout;
}

void Turtle_AutoCtrl(AutoCtrl_StateTypeDef *hPid)
{
	//如果未使能，直接退出
	if(hPid->enable == AUTO_DISABLE)
	{
		//hPid->esum = 0;
		return;
	}
	//误差获取
	hPid->err[2] = hPid->set_value - hPid->act_value;
	if (hPid->err[2] <= hPid->out_db && hPid->err[2] >= -hPid->out_db)
		return;
	//误差值处理:临界值处理
	switch(hPid->symbol)
	{
		case AUTO_DEPTH:
		{
			//此处暂时为空
			break;
		}
		case AUTO_HEADING:
		{
			if (hPid->err[2]>1800)
				hPid->err[2] = hPid->err[2]-3600;
			else if (hPid->err[2]<-1800)
				hPid->err[2] = hPid->err[2]+3600;
			break;
		}
		case AUTO_PITCH: 
		{
			if (hPid->err[2]>1800)
				hPid->err[2] = hPid->err[2]-3600;
			else if (hPid->err[2]<-1800)
				hPid->err[2] = hPid->err[2]+3600;
			break;
		}
		case AUTO_ROLL: 
		{
			if (hPid->err[2]>1800)
				hPid->err[2] = hPid->err[2]-3600;
			else if (hPid->err[2]<-1800)
				hPid->err[2] = hPid->err[2]+3600;
			break;
		}
		default:
			break;
	}
	//PID计算
	switch (hPid->mode)
	{
		case MODE_PID:
			Turtle_AutoCtrl_PID(hPid);
			break;
		case MODE_SGPID:
			Turtle_AutoCtrl_SGPID(hPid);
			break;
		case MODE_SCPID:
			Turtle_AutoCtrl_PID(hPid);
			hPid->pidout += hPid->out_dc;
			break;
		case MODE_CCPID:
			if (hPid->symbol == AUTO_DEPTH)
				Turtle_Auto_CCPID();
			break;
		default:
			break;
	}
	//控制量非线性处理
	switch (hPid->out_mode)
	{
		case OUT_ORG:
			PID_OutOrg(hPid);
			break;
		case OUT_XGATE:
			//此处暂时为空
			break;
		case OUT_YGATE:
			//此处暂时为空
			break;
		case OUT_SQRT:
			if (hPid->pidout > out_sqrt_gate)
				hPid->pidout = sqrt(hPid->pidout);
			PID_OutOrg(hPid);
			break;
		case OUT_DC:
			if (hPid->symbol == AUTO_DEPTH)
				hPid->pidout += hPid->out_dc;
			PID_OutOrg(hPid);
			break;
		default:
			break;
	}
	//电机控制量转化
	switch(hPid->symbol)
	{
		uint8_t i;
		uint8_t sign;
		uint8_t offset;
		case AUTO_DEPTH:
			Turtle_Prop_UpDown(out_mid + (short)hPid->pidout);
			break;
		case AUTO_HEADING:
			Turtle_Prop_FHeading(out_mid + (short)hPid->pidout);
			break;
		case AUTO_ROLL:
			if (at_en == 1)
				Turtle_AutoCtrl_At();
			break;
		default:
			break;
	}
}


/****************差分进化算法函数****************/
float   h 			=  0.1;
float   B[3][4] = {{ 0.98803162, -0.98803162, -0.98803162,  0.98803162},
									 {-0.15425145, -0.15425145,  0.15425145,  0.15425145},
									 { 1.0,         1.0,         1.0,         1.0} };

static float float_abs(float buf)
{
	if (buf<0)
		buf = -buf;

	return buf;
}

static float function_fitness(float* var)
{
	float result = 0;
	float result_buf[3] = {0};
	
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result_buf[i] += B[i][j] * var[j];
		}
		result_buf[i] -= u[i];
	}

	for (int i = 0; i < 3; i++)
	{
		result += float_abs(result_buf[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		result += float_abs(var[i]);
	}

	return result;
}

//姿态控制函数
static uint8_t m_buf[4] = {0};
	
void Turtle_AutoCtrl_At(void)
{
	if (de_en == 1)
	{
		u[0] = hAuto[3].pidout;
		u[1] = hAuto[2].pidout;
		u[2] = hAuto[0].pidout*4;
		
		runde(0.8, 0.4, -125.0, 125.0, function_fitness);
		
		for (uint8_t i=0;i<4;i++)
			m_buf[i] = (uint8_t)((short)de.global_solution[i] + out_mid);
		Turtle_Prop_AtDrt(m_buf);
	}
	else
	{
		hAuto[AUTO_PITCH_ID].pidout *=0.2;
		hAuto[AUTO_ROLL_ID].pidout *=0.2;
		
//		m_buf[0] = (uint8_t)(out_mid - hAuto[AUTO_PITCH_ID].pidout);
//		m_buf[1] = (uint8_t)(out_mid + hAuto[AUTO_PITCH_ID].pidout - hAuto[AUTO_ROLL_ID].pidout);
//		m_buf[2] = (uint8_t)(out_mid + hAuto[AUTO_ROLL_ID].pidout);
//		m_buf[3] = (uint8_t)out_mid;
		float beta = hAuto[AUTO_PITCH_ID].pidout;
		float gamma = hAuto[AUTO_ROLL_ID].pidout;
		
		m_buf[0] = (uint8_t)(out_mid - 0.5*beta - 0.2887*gamma);
		m_buf[1] = (uint8_t)(out_mid + 0.5*beta - 0.2887*gamma);
		m_buf[2] = (uint8_t)(out_mid + 0.5*beta + 0.2887*gamma);
		m_buf[3] = (uint8_t)(out_mid - 0.5*beta + 0.2887*gamma);
		
		Turtle_Prop_AtPlus(m_buf);
	}
}

void Turtle_AutoCtrl_EnAt(void)
{
	hAuto[AUTO_PITCH_ID].set_value = 0;
	hAuto[AUTO_ROLL_ID].set_value = 0;
	hAuto[AUTO_PITCH_ID].enable = AUTO_ENABLE;
	hAuto[AUTO_ROLL_ID].enable = AUTO_ENABLE;
}

void Turtle_AutoCtrl_DisAt(void)
{
	hAuto[AUTO_PITCH_ID].enable = AUTO_DISABLE;
	hAuto[AUTO_ROLL_ID].enable = AUTO_DISABLE;
}

/****************自动控制算法函数****************/

//PID控制
void Turtle_AutoCtrl_PID(AutoCtrl_StateTypeDef *hPid)
{
	uint8_t i;
	
	hPid->esum += hPid->err[2];
	hPid->pidout = hPid->kp*(hPid->err[2] + hPid->ki*CYCLE*hPid->esum\
								 + hPid->kd/CYCLE*(hPid->err[2] - hPid->err[1]));
	for (i=0;i<2;i++)
		hPid->err[i] = hPid->err[i+1];
	hPid->err[2] = 0;
	//pidout[k] = Kp*(e[k] + T / Ti*esum[k] + Td / T*(e[k] - e[k - 1]));
}

//分段式PID参数
uint8_t sg_maxpow = 3;			//分段PID分段数 = SG_MAXPOW-1
uint8_t sg_step		= 2;			//分段PID步长
float   sg_kp_bs  = 0.8;		//KP基数
float   sg_kd_bs  = 0.9;		//KD基数
float   sg_ki_bs	= 1;			//KI基数

//分段PID控制
float pid_buf[3];

void Turtle_AutoCtrl_SGPID(AutoCtrl_StateTypeDef *hPid)
{
	uint8_t i;
	uint8_t p;
	
	p = (uint8_t)(hPid->err[2]/sg_step);
	if (p < sg_maxpow)
		p = sg_maxpow - p;
	else
		p = 1;
	pid_buf[0] = hPid->kp*rov_pow(sg_kp_bs,p);
	pid_buf[1] = hPid->ki*rov_pow(sg_ki_bs,p);
	pid_buf[2] = hPid->kd*rov_pow(sg_kd_bs,p);
	hPid->esum += hPid->err[2];
	hPid->pidout = pid_buf[0]*(hPid->err[2] + pid_buf[1]*CYCLE*hPid->esum\
								 + pid_buf[2]/CYCLE*(hPid->err[2] - hPid->err[1]));
	for (i=0;i<2;i++)
		hPid->err[i] = hPid->err[i+1];
	hPid->err[2] = 0;
}

/****************系统辨识相关****************/

SI_StateTypeDef hSI[4];

#define PI		3.1415926f

static float input_sin_k = 2*PI*CYCLE;
//系统辨识初始化
void Turtle_SI_Init(void)
{
	uint8_t i;
	
	for(i=0;i<4;i++)
	{
		hSI[i].amp		= 125;
		hSI[i].enable = SI_DISABLE;
		hSI[i].mode		= 1<<i;
		hSI[i].si_outmode = SI_SIN; 
	}
}

//系统辨识
void Turtle_SI(SI_StateTypeDef *hSI)
{
	static uint32_t si_cnt = 0;
	static float t = 0;
	
	if (hSI->enable == SI_DISABLE)
		return;
	
	hSI->si_out = hSI->amp*sin(input_sin_k/hSI->period*t);
	
	switch(hSI->mode)
	{
		case SI_DEPTH:
			Turtle_Prop_UpDown (out_mid + (short)hSI->si_out);
			break;
		case SI_HEADING:
			Turtle_Prop_FHeading(out_mid + (short)hSI->si_out);
			break;
		case SI_PITCH:
			Turtle_Prop_Pitch(out_mid + (short)(hSI->si_out));
			break;
		case SI_ROLL:
			Turtle_Prop_Heading(out_mid + (short)(hSI->si_out));
			break;
	}
	
	t += 1;
}
