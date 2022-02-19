#ifndef TURTLE_FOPID_TYPE_H
#define TURTLE_FOPID_TYPE_H

#define USR_FO_N      10u

#include "stdint.h"

typedef   enum    TPidMode      TPidMode_Type;
typedef   enum    TPidFlag      TPidFlag_Type;
typedef   enum    TPidOut       TPidOut_Type;
typedef   struct  TurtlePid     TurtlePid_Type;

enum TPidMode{
  MODE_PID      = 0x10010000,
  MODE_FOPID    = 0x10010001,
  MODE_CCPID    = 0x10010002,
};

enum TPidFlag{
  FLAG_NULL     = 0x10020000,
  FLAG_DEPTH    = 0x10020001,
  FLAG_HEADING  = 0x10020002,
  FLAG_PITCH    = 0x10020004,
  FLAG_ROLL     = 0x10020008,
};

enum TPidOut
{
  OUT_ORG       = 0x10030000,
  OUT_DC        = 0x10030001,
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
  float                     out_dc;
  TPidOut_Type              out_mode;
  /*配置参数*/
  uint8_t                   pid_enable;
  TPidMode_Type             pid_mode;
  TPidFlag_Type             pid_flag;              
};

#endif
