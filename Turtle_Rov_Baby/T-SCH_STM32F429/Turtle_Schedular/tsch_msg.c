/**
  ******************************************************************************
  * @file    tsch_msg.c
  * @author  XuczSnow, OUC/Fab U+
  * @brief   Turtle Scheduler 消息栈及消息队列文件，主要包含了创建和处理函数
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
#include "tsch_msg.h"

TSchMsg_Type    *__tsch_msg_current;

/**
  * @brief  任务消息创建
  *
  * @param  msg     消息量处理块
  * @param  mode    消息量处理模式，包含消息队列及消息栈两种
  *                 @arg MSG_STACK 消息栈，FILO
  *                 @arg MSG_QUEUE 消息队列。FIFO
  * @param  buf     消息量存储空间
  * @param  length  消息量缓存长度
  * 
  * @retval TSchResState见定义
  */
TSchResState_Type TSch_MsgCreat(TSchMsg_Type *msg, TSchMsgMode_Type mode, TSchMsgEle_Type *buf, uint16_t length){
  msg->msg_length = length;
  msg->msg_mode = mode;
  msg->msg = buf;
  return TSCH_OK;
}

/**
  * @brief  任务消息获取
  *
  * @param  msg     消息量处理块
  * @param  element 消息量存储缓存
  * @param  len     消息量获取长度
  * 
  * @retval TSchResState见定义
  */
TSchResState_Type TSch_MsgGet(TSchMsg_Type *msg, TSchMsgEle_Type *element, uint16_t len){
  uint16_t head = msg->msg_head;
  uint16_t tail = msg->msg_tail;
  uint16_t count = msg->msg_count;
  uint16_t length = msg->msg_mode;

  if (count != head-tail || count != tail+length-head) return TSCH_INVAILD;

  switch (msg->msg_mode){
    case MSG_STACK:{
      for (uint16_t i=0;i<len;++i){
        element[i] = msg->msg[head--];
        if (head == 0) return TSCH_EMPTY;
      }
      break;
    }
    case MSG_QUEUE:{
      for (uint16_t i=0;i<len;++i){
        element[i] = msg->msg[head--];
        if (head == 0) head = length;
        if (head == tail) return TSCH_EMPTY;
      }
      break;
    }
    default:{
      return TSCH_INVAILD;
    }
  }
  return TSCH_OK;
}

/**
  * @brief  任务消息发布
  *
  * @param  msg     消息量处理块
  * @param  element 消息量存储缓存
  * @param  len     消息量发布长度
  * 
  * @retval TSchResState见定义
  */
TSchResState_Type TSch_MsgPub(TSchMsg_Type *msg, TSchMsgEle_Type *element, uint16_t len){
  uint16_t head = msg->msg_head;
  uint16_t tail = msg->msg_tail;
  uint16_t count = msg->msg_count;
  uint16_t length = msg->msg_mode;

  if (count != head-tail || count != tail+length-head) return TSCH_INVAILD;

  switch(msg->msg_mode){
    case MSG_STACK:{
      for (uint16_t i=0;i<len;++i){
        if (head == length-1) return TSCH_FULL;
        msg->msg[++head] = element[i];
      }
      break;
    }
    case MSG_QUEUE:{
       for (uint16_t i=0;i<len;++i){
        if (head == tail-1) return TSCH_FULL;
        if (head == length) head = 0;
        msg->msg[++head] = element[i];
      }
      break;
    }
    default:{
      return TSCH_INVAILD;
    }
  }
  msg->tmr_last = TSch_TmrGet();
  if (msg->task_wait != NULL){
    __tsch_msg_current = msg;
    if (msg->task_wait->syn_funptr !=NULL)
      msg->task_wait->syn_funptr(msg->task_wait->task_prio);
  }
  return TSCH_OK;
}
