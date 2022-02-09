/**
  ******************************************************************************
  * @file    tsch.c
  * @author  XuczSnow, OUC/Fab U+
  * @brief   Turtle Scheduler TSch文件
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
#include "tsch.h"

/**
  * @brief  用户空闲任务函数
  *
  * @note   如用户需要在空闲任务中加入自行处理的数据，可以重载此函数
  */
__weak void TSch_IdleTaskUsr(void){
    return;
}

/**
  * @brief  系统空闲任务
  */
static void __TSch_IdleTask(void){
    TSch_IdleTaskUsr();
    return;
}

/**
  * @brief  时间调度器启动函数
  *
  * @note   初始化完成后，使用此函数，任务调度器将接管系统
  */
void TSch_Start(void){
    while(1){
        __TSch_IdleTask();
    }
}
