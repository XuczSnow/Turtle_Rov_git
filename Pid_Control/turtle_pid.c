#include "turtle_pid.h"

#include "math.h"

#include "turtle_pid_type.h"

void Turtle_PidInit(TurtlePid_Type *pid, float Kp, float Ki, float Kd, float T, \
                    float lam, float u, float min, float max, TurtlePidMode_Type mode,\
                    TurtlePidFlag_Type flag){
  pid->pid_Mode = mode, pid->pid_Flag = flag;
  pid->out_min = min, pid->out_max = max;
  pid->pid_Kp = Kp, pid->pid_Ki = Ki, pid->pid_Kd = Kd, pid->pid_T = T;
  if (mode == MODE_FOPID){
    pid->fopid_lam = lam, pid->fopid_u = u;
    /*初始化Fopid参数*/
    for (uint8_t i=0;i<USR_FO_N;i++){
      if (i=0){
        pid->err_wl[i] = 1;
        pid->err_wu[i] = 1;
      }
      else{
        pid->err_wl[i] = (1-(lam+1)/i)*(pid->err_wl[i-1]);
        pid->err_wu[i] = (1-(u+1)/i)*(pid->err_wu[i-1]);
      }
    }
    pid->fopid_Tlam = pow(T, lam);
    pid->fopid_Tu   = pow(T, u);
  }
  return;
}

void Turtle_PidFresh(){

}

void Turtle_Fopid(TurtlePid_Type *fopid){
  float ki_temp = 0;
  float kd_temp = 0;

  for (uint8_t i=USR_FO_N;i>=0;--i) fopid->err[i+1] = fopid->err[i];
  fopid->err[0] = fopid->pid_setpoint - fopid->pid_actualpoint;
  for (uint8_t i=0;i<USR_FO_N;++i){
    ki_temp += (fopid->err_wl[i])*(fopid->err[i]);
    kd_temp += (fopid->err_wu[i])*(fopid->err[i]);
  }
  fopid->pid_output = fopid->pid_Kp*fopid->err[0]+\
                      fopid->pid_Ki*fopid->fopid_Tlam*ki_temp+\
                      fopid->pid_Kd*fopid->fopid_Tu*kd_temp;
}

void TurTle_Pid(TurtlePid_Type *pid){
  for (uint8_t i=USR_FO_N;i>=0;--i) pid->err[i+1] = pid->err[i];
  pid->err[0] = pid->pid_setpoint = pid->pid_actualpoint;
  pid->err_sum += pid->err[0];
  pid->pid_output = pid->pid_Kp*pid->err[0]+\
                    pid->pid_Ki*pid->pid_T*pid->err_sum+\
                    pid->pid_Kd/pid->pid_T*(pid->err[0]-pid->err[1]);
}

void Turtle_CtrlRun(){
  
}
