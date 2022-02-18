/**
  ******************************************************************************
  * @file    turtle_fifo.h
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 的FIFO相关函数头文件
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
	
#ifndef TURTLE_FIFO_H
#define TURTLE_FIFO_H

/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

//Queue Initalize
void QueueInit(FifoQueueTypedef *Queue);
// Queue In
uint8_t QueueIn(FifoQueueTypedef *Queue,ElemType sdat);
// Queue Out
uint8_t QueueOut(FifoQueueTypedef *Queue,ElemType *sdat);

#endif