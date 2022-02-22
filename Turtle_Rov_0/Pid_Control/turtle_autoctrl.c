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
#include "turtle_autoctrl.h"

#include "math.h"
#include "stdlib.h"

//#include "de.h"
#include "turtle_g.h"

#define CYCLE        0.2f
#define CTRL_MODE    MODE_PID

TurtlePid_Type hAuto[4];
TurtlePid_Type hDepthAuto[2];

float u[3]  =  {0};      //控制量缓存

uint8_t de_en = 0;

float pid_kp[4] = {6.0,  0.1,  0.4,  0.0};
float pid_ki[4] = {0.3,  0.4,    0,  0.0};
float pid_kd[4] = {2.5,  0.4,  0.4,  0.0};

float fopid_lam[4] = {1.0,  1.0,  1.0,  1.0};
float fopid_u[4]   = {1.0,  1.0,  1.0,  1.0};

TPidMode_Type pid_mode[4] = {MODE_FOPID,MODE_FOPID,MODE_FOPID,MODE_FOPID};
TPidOut_Type  pid_out[4]  = {OUT_DC,OUT_ORG,OUT_ORG,OUT_ORG};

float depth_kp[2] = {0};
float depth_ki[2] = {0};
float depth_kd[2] = {0};

float depth_lam[2] = {1.0,  1.0};
float depth_u[2]   = {1.0,  1.0};

float depth_dc = 36;

TPidMode_Type depth_mode[2] = {MODE_PID,MODE_FOPID};
TPidOut_Type  depth_out[2]  = {OUT_ORG,OUT_DC};

void Turtle_AutoCtrl_Init(void)
{
  uint8_t i;
  uint8_t pid_buf[6];

  for (i=0;i<4;i++)
  {
    Turtle_PidInit(&hAuto[i], pid_kp[i], pid_ki[i], pid_kd[i], CYCLE,\
                   fopid_lam[i], fopid_u[i], 0, 0, pid_out[i],\
                   pid_mode[i],FLAG_NULL|(0x00000001<<i));
    hAuto[i].pid_enable = AUTO_DISABLE;
    HAL_Delay(10);
  }
  for (i=0;i<2;i++){
    Turtle_PidInit(&hDepthAuto[i], depth_kp[i], depth_ki[i], depth_kd[i], CYCLE,\
                   depth_lam[i], depth_u[i], 0, 0, depth_out[i],\
                   depth_mode[i], FLAG_DEPTH);
    hAuto[i].pid_enable = AUTO_DISABLE;
    HAL_Delay(10);
  }
}

/*PID控制参数刷新*/
void Turtle_AutoCtrl_Refresh(void)
{
  //刷新PID实际值
  hAuto[AUTO_DEPTH_ID].pid_actualpoint    = (float)hPress.conv_depth[0]*0.1;
  hAuto[AUTO_HEADING_ID].pid_actualpoint  = hRov.yaw*10;
  hAuto[AUTO_PITCH_ID].pid_actualpoint    = hRov.pitch*10;
  hAuto[AUTO_ROLL_ID].pid_actualpoint     = hRov.roll*10;
  
  //刷新串级PID实际值
  hDepthAuto[0].pid_actualpoint = (float)hPress.conv_depth[0]*0.1;
  hDepthAuto[1].pid_actualpoint = hPress.conv_speed[0];
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

void PID_OutOrg(TurtlePid_Type *hPid)
{
  float u_buf;
  u_buf = hPid->pid_output;
  if (u_buf > out_max)
    u_buf = out_max;
  else if(u_buf < out_gate && u_buf > -out_gate)
    u_buf = 0;
  else if (u_buf < -out_max)
    u_buf = -out_max;
  
  hPid->pid_output = u_buf;
}

static void Depth_OutDc(TurtlePid_Type *hPid)
{
  float u_buf;
  
  u_buf = hPid->pid_output;
  u_buf += hPid->out_dc;
  if (u_buf > out_max)
    u_buf = out_max;
  else if (u_buf < 0)
  {
    //u_buf /=0.7;
    if (u_buf < -(out_max-hPid->out_dc))
      u_buf = -(out_max-hPid->out_dc);
  }
  hPid->pid_output = u_buf;
}

static void Turtle_Auto_CCPID(void)
{
  hDepthAuto[0].pid_enable = AUTO_ENABLE;
  hDepthAuto[1].pid_enable = AUTO_ENABLE;
  //外环PID计算
  hDepthAuto[0].pid_setpoint = hAuto[0].pid_setpoint;
  Turtle_AutoCtrl(&hDepthAuto[0]);
  
  //内环PID计算
  hDepthAuto[1].pid_setpoint = hDepthAuto[0].pid_output;
  Turtle_AutoCtrl(&hDepthAuto[1]);
  
  hAuto[0].pid_output = hDepthAuto[1].pid_output;
}

void Turtle_AutoCtrl(TurtlePid_Type *hPid)
{
  float err_temp;
  //如果未使能，直接退出
  if(hPid->pid_enable == AUTO_DISABLE)
  {
    hPid->err_sum = 0;
    return;
  }
  //误差获取
  err_temp = hPid->pid_setpoint - hPid->pid_actualpoint;
  //误差值处理:临界值处理
  switch(hPid->pid_flag)
  {
    case FLAG_DEPTH:
    {
      //此处暂时为空
      break;
    }
    case FLAG_HEADING:
    {
      if (err_temp>1800)
        err_temp = err_temp-3600;
      else if (err_temp<-1800)
        err_temp = err_temp+3600;
      break;
    }
    case FLAG_PITCH: 
    {
      if (err_temp>1800)
        err_temp = err_temp-3600;
      else if (err_temp<-1800)
        err_temp = err_temp+3600;
      break;
    }
    case FLAG_ROLL: 
    {
      if (err_temp>1800)
        err_temp = err_temp-3600;
      else if (err_temp<-1800)
        err_temp = err_temp+3600;
      break;
    }
    default:
      break;
  }
  //PID计算
  switch (hPid->pid_mode)
  {
    case MODE_PID:
      TurTle_Pid(hPid, err_temp);
      break;
    case MODE_FOPID:
      Turtle_Fopid(hPid, err_temp);
      break;
    case MODE_CCPID:
      //if (hPid->pid_flag == FLAG_DEPTH)
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
    case OUT_DC:
      if (hPid->pid_flag == FLAG_DEPTH)
      {
        hPid->out_dc = depth_dc;
        Depth_OutDc(hPid);
      }
      break;
    default:
      break;
  }
  //电机控制量转化
  switch(hPid->pid_flag)
  {
    uint8_t i;
    uint8_t sign;
    uint8_t offset;
    case FLAG_DEPTH:
      Turtle_Prop_UpDown((uint8_t)(out_mid + hPid->pid_output));
      break;
    case FLAG_HEADING:
      Turtle_Prop_FHeading((uint8_t)(out_mid + hPid->pid_output));
      break;
    case FLAG_ROLL:
      if (at_en == 1)
        Turtle_AutoCtrl_At();
      break;
    default:
      break;
  }
}


/****************差分进化算法函数****************/
/* @note 旧的四维差分进化算法                   */
float   h       =  0.1;
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
#if 0 /*差分进化算法，暂未使用*/
  if (de_en == 1)
  {
    u[0] = hAuto[3].pid_output;
    u[1] = hAuto[2].pid_output;
    u[2] = hAuto[0].pid_output * 4;
    
    runde(0.8, 0.4, -125.0, 125.0, function_fitness);
    
    for (uint8_t i=0;i<4;i++)
      m_buf[i] = (uint8_t)((short)de.global_solution[i] + out_mid);
    Turtle_Prop_AtDrt(m_buf);
  }
#endif 
//  else
//  {
#if 0 /*旧的二维计算方法*/
    hAuto[AUTO_PITCH_ID].pidout *=0.2;
    hAuto[AUTO_ROLL_ID].pidout *=0.2;
    
    m_buf[0] = (uint8_t)(out_mid - hAuto[AUTO_PITCH_ID].pidout);
    m_buf[1] = (uint8_t)(out_mid + hAuto[AUTO_PITCH_ID].pidout - hAuto[AUTO_ROLL_ID].pidout);
    m_buf[2] = (uint8_t)(out_mid + hAuto[AUTO_ROLL_ID].pidout);
    m_buf[3] = (uint8_t)out_mid;
#endif

    float beta = hAuto[AUTO_PITCH_ID].pid_output * 0.2;
    float gamma = hAuto[AUTO_ROLL_ID].pid_output * 0.2;
    
    m_buf[0] = (uint8_t)(out_mid - 0.5*beta - 0.2887*gamma);
    m_buf[1] = (uint8_t)(out_mid + 0.5*beta - 0.2887*gamma);
    m_buf[2] = (uint8_t)(out_mid + 0.5*beta + 0.2887*gamma);
    m_buf[3] = (uint8_t)(out_mid - 0.5*beta + 0.2887*gamma);
    
    Turtle_Prop_AtPlus(m_buf);
//  }
}

void Turtle_AutoCtrl_EnAt(void)
{
  hAuto[AUTO_PITCH_ID].pid_setpoint = 0;
  hAuto[AUTO_ROLL_ID].pid_setpoint = 0;
  hAuto[AUTO_PITCH_ID].pid_enable = AUTO_ENABLE;
  hAuto[AUTO_ROLL_ID].pid_enable = AUTO_ENABLE;
}

void Turtle_AutoCtrl_DisAt(void)
{
  hAuto[AUTO_PITCH_ID].pid_enable = AUTO_DISABLE;
  hAuto[AUTO_ROLL_ID].pid_enable = AUTO_DISABLE;
}

/****************自动控制算法函数****************/
#if 0 //旧的分段式PID
//分段式PID参数
uint8_t sg_maxpow = 3;      //分段PID分段数 = SG_MAXPOW-1
uint8_t sg_step    = 2;      //分段PID步长
float   sg_kp_bs  = 0.8;    //KP基数
float   sg_kd_bs  = 0.9;    //KD基数
float   sg_ki_bs  = 1;      //KI基数

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
#endif

/****************系统辨识相关****************/
#if 0 //系统辨识相关算法，未移植
SI_StateTypeDef hSI[4];

#define PI    3.1415926f

static float input_sin_k = 2*PI*CYCLE;
//系统辨识初始化
void Turtle_SI_Init(void)
{
  uint8_t i;
  
  for(i=0;i<4;i++)
  {
    hSI[i].amp    = 125;
    hSI[i].enable = SI_DISABLE;
    hSI[i].mode    = 1<<i;
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
#endif
