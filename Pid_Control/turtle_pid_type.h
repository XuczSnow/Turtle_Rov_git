#ifndef TURTLE_FOPID_TYPE_H
#define TURTLE_FOPID_TYPE_H

#define     USR_FO_N      10u

#include "stdint.h"

typedef   enum    TurtlePidMode     TurtlePidMode_Type;
typedef   enum    TurtlePidFlag     TurtlePidFlag_Type;
typedef   struct  TurtlePid         TurtlePid_Type;

enum TurtlePidMode{
  MODE_PID      = 0x10010000,
  MODE_FOPID    = 0x10010001,
};

enum TurtlrPidFlag{
  FLAG_NULL     = 0x10020000,
  FLAG_DEPTH    = 0x10020001,
  FLAG_HEADING  = 0x10020002,
  FLAG_PITCH    = 0x10020003,
  FLAG_ROLL     = 0x10020004,
};

struct TurtlePid
{
  /*输入输出*/
  float                     pid_setpoint;
  float                     pid_actualpoint;
  float                     pid_output;
  /*PID计算参数*/
  float                     pid_Kp;
  float                     pid_Ki;
  float                     pid_Kd;
  float                     pid_T;
    /*Fopid参数*/
  float                     fopid_lam;
  float                     fopid_u;
  float                     fopid_Tlam;
  float                     fopid_Tu;
  /*误差参数*/
  float                     err[USR_FO_N];
  float                     err_sum;
  float                     err_wl[USR_FO_N];
  float                     err_wu[USR_FO_N];
  /*其他参数*/
  float                     out_min;
  float                     out_max;
  /*配置参数*/
  uint8_t                   pid_enable;
  TurtlePidMode_Type        pid_Mode;
  TurtlePidFlag_Type        pid_Flag;
};


#endif
