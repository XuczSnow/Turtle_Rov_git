
#ifndef TURTLE_PID_H
#define TURTLE_PID_H

#include "turtle_pid_type.h"

void Turtle_PidInit(TurtlePid_Type *pid, float Kp, float Ki, float Kd, float T, \
                    float lam, float u, float min, float max, TPidOut_Type out_mode,\
                    TPidMode_Type mode, TPidFlag_Type flag);
void Turtle_Fopid(TurtlePid_Type *fopid, float err);
void TurTle_Pid(TurtlePid_Type *pid, float err);

#endif