/**
  ******************************************************************************
  * @file    turtle_pid.c
  * @author  XuczSnow, OUC/Fab U+
  * @brief   Turtle PID处理文件，包含FOPID及PID
  * 
  @verbatim

  @endverbatim
  ******************************************************************************
  @attention

  MIT License

  Copyright (c) 2022 XuczSnow

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  ******************************************************************************
  */

#include "turtle_pid.h"

#include "math.h"

#include "turtle_pid_type.h"

void Turtle_PidInit(TurtlePid_Type *pid, float Kp, float Ki, float Kd, float T, \
                    float lam, float u, float min, float max, TPidOut_Type out_mode,\
                    TPidMode_Type mode, TPidFlag_Type flag){
  pid->pid_mode = mode, pid->pid_flag = flag;
  pid->out_min = min, pid->out_max = max, pid->out_mode = out_mode;
  pid->pid_Kp = Kp, pid->pid_Ki = Ki, pid->pid_Kd = Kd, pid->pid_T = T;
  if (mode == MODE_FOPID){
    pid->fopid_lam = lam, pid->fopid_u = u;
    /*初始化Fopid参数*/
    Turtle_FopidInit(pid);
  }
  return;
}

void Turtle_FopidInit(TurtlePid_Type *pid){
  /*初始化Fopid参数*/
  for (uint8_t i=0;i<USR_FO_N;++i){
    if (i==0){
      pid->err_wl[i] = 1;
      pid->err_wu[i] = 1;
    }
    else{
      pid->err_wl[i] = (1-(-pid->fopid_lam+1)/i)*(pid->err_wl[i-1]);
      pid->err_wu[i] = (1-(pid->fopid_u+1)/i)*(pid->err_wu[i-1]);
    }
  }
  pid->fopid_Tlam = pow(pid->pid_T, pid->fopid_lam);
  pid->fopid_Tu   = pow(pid->pid_T, -pid->fopid_u);
}

void Turtle_Fopid(TurtlePid_Type *fopid, float err){
  float ki_temp = 0;
  float kd_temp = 0;

  for (uint8_t i=USR_FO_N;i>0;--i) fopid->err[i] = fopid->err[i-1];
  fopid->err[0] = err;
  for (uint8_t i=0;i<USR_FO_N;++i){
    ki_temp += (fopid->err_wl[i])*(fopid->err[i]);
    kd_temp += (fopid->err_wu[i])*(fopid->err[i]);
  }
  fopid->pid_output = fopid->pid_Kp*fopid->err[0]+\
                      fopid->pid_Ki*fopid->fopid_Tlam*ki_temp+\
                      fopid->pid_Kd*fopid->fopid_Tu*kd_temp;
  if (fopid->out_max == 0 && fopid->out_min == fopid->out_max) return;
  if (fopid->pid_output > fopid->out_max) fopid->pid_output = fopid->out_max;
  else if (fopid->pid_output < fopid->out_min) fopid->pid_output = fopid->out_min;
}

void TurTle_Pid(TurtlePid_Type *pid, float err){
  for (uint8_t i=USR_FO_N;i>0;--i) pid->err[i] = pid->err[i-1];
  pid->err[0] = err;
  pid->err_sum += pid->err[0];
  pid->pid_output = pid->pid_Kp*pid->err[0]+\
                    pid->pid_Ki*pid->pid_T*pid->err_sum+\
                    pid->pid_Kd/pid->pid_T*(pid->err[0]-pid->err[1]);
  if (pid->out_max == 0 && pid->out_min == pid->out_max) return;
  if (pid->pid_output > pid->out_max) pid->pid_output = pid->out_max;
  else if (pid->pid_output < pid->out_min) pid->pid_output = pid->out_min;
}
