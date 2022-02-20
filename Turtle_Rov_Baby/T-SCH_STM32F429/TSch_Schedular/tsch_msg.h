/**
  ******************************************************************************
  * @file    tsch_msg.h
  * @author  XuczSnow, OUC/Fab U+
  * @brief   Turtle Scheduler 消息栈及消息队列头文件
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
#ifndef TSCH_MSG_H
#define TSCH_MSG_H

#include "tsch_global.h"

/*************************************宏定义声明**********************************/

#define   MSG_NULL       NULL      /*任务无需消息阻塞时使用*/

extern TSchMsg_Type    *__tsch_msg_current;

/**************************************函数声明**********************************/

TSchResState_Type TSch_MsgCreat(TSchMsg_Type *msg, TSchMsgMode_Type mode, TSchMsgEle_Type *buf, uint16_t length);
TSchResState_Type TSch_MsgGet(TSchMsg_Type *msg, TSchMsgEle_Type *element, uint16_t len);
TSchResState_Type TSch_MsgPub(TSchMsg_Type *msg, TSchMsgEle_Type *element, uint16_t len);

#endif
/*************************************头文件结束**********************************/
