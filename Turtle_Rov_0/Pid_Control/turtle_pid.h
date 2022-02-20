/**
  ******************************************************************************
  * @file    turtle_pid.c
  * @author  XuczSnow, OUC/Fab U+
  * @brief   Turtle PID处理头文件
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

#ifndef TURTLE_PID_H
#define TURTLE_PID_H

#include "turtle_pid_type.h"

void Turtle_PidInit(TurtlePid_Type *pid, float Kp, float Ki, float Kd, float T, \
                    float lam, float u, float min, float max, TPidOut_Type out_mode,\
                    TPidMode_Type mode, TPidFlag_Type flag);
void Turtle_FopidInit(TurtlePid_Type *pid);
void Turtle_Fopid(TurtlePid_Type *fopid, float err);
void TurTle_Pid(TurtlePid_Type *pid, float err);

#endif