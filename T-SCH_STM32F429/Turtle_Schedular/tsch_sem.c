/**
  ******************************************************************************
  * @file    tsch_sem.c
  * @author  XuczSnow, OUC/Fab U+
  * @brief   Turtle Scheduler 信号量处理文件
  *
  @verbatim
  1. 信号量为一个计数值，且所有信号量发布时间戳均会被记录，时间戳记录数据存储方式为：从
  前到后对应信号量发布时间从早到晚，获取信号量是会获取最早发布的信号量
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

#include "tsch_sem.h"

/**
  * @brief  信号量设置
  *
  * @param  sem     信号量处理块
  * @param  num     信号量数量
  * 
  * @retval TSchResState见定义
  * 
  * @note   该函数会将信号量设置为指定值，并且所有时间戳为当前设置时间戳
  */
TSchResState_Type TSch_SemSet(TSchSem_Type *sem, uint8_t num){
  TSchTmr_Type  tmr;
  sem->sem_cnt = num;
  tmr = TSch_TmrGet();
  for (int i=0;i<num;i++) sem->tmr[i] = tmr;
  return TSCH_OK;
}

/**
  * @brief  信号量增加
  *
  * @param  sem     信号量处理块
  * @param  num     信号量大小
  * 
  * @retval TSchResState见定义
  * 
  * @note   该函数会再原信号量的基础上增加指定值，并且增加信号量的时间戳赋值为当前时间戳
  */
TSchResState_Type TSch_SemAdd(TSchSem_Type *sem, uint8_t num){
  TSchTmr_Type  tmr;
  uint8_t       temp_cnt;
  temp_cnt = sem->sem_cnt;
  sem->sem_cnt += num;
  tmr = TSch_TmrGet();
  for (int i=0;i<num;i++) sem->tmr[temp_cnt+i] = tmr;
  return TSCH_OK;
}

/**
  * @brief  信号量获取
  *
  * @param  sem     信号量处理块
  * @param  tmr     当前信号量时间戳
  * 
  * @retval TSchResState见定义
  * 
  * @note   根据信号量定义，每次只能获取一个信号量，若信号量为空，返回TSCH_EMPTY
  */
TSchResState_Type TSch_SemGet(TSchSem_Type *sem, TSchTmr_Type *tmr){
  if (sem->sem_cnt == 0) return TSCH_EMPTY;
  for (int i=0;i<sem->sem_cnt;++i)  sem->tmr[i-1] = sem->tmr[i];
  sem->sem_cnt --;
  sem->tmr[sem->sem_cnt] = 0;
  return TSCH_OK;
}
