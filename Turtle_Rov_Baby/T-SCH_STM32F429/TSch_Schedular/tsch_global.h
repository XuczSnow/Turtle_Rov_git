/**
  ******************************************************************************
  * @file    tsch_global.h
  * @author  XuczSnow, OUC/Fab U+
  * @brief   Turtle Scheduler全局头文件，主要涵盖了头文件包含等
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

#ifndef STCH_GLOBAL_H
#define STCH_GLOBAL_H

/******************************相关宏定义***************************/
#ifndef __weak
  #define __weak  __attribute__((weak))     /*函数重载定义*/
#endif

#ifndef NULL
  #define NULL (void *)0                    /*空指针定义*/
#endif

/******************************头文件包含***************************/

#include "stdint.h"

#include "tsch_usr_conf.h"
#include "tsch_type_def.h"

#include "tsch.h"
#include "tsch_msg.h"
#include "tsch_sch.h"
#include "tsch_task.h"
#include "tsch_tmr.h"


#endif

/******************************头文件结束***************************/
