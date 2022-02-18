/**
  ******************************************************************************
  * @file    turtle_fifo.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle 系列 ROV 的FIFO相关函数
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "turtle_g.h"

FifoQueueTypedef Terminal_RxQueue;

//Queue Init
void QueueInit(FifoQueueTypedef *Queue)
{
    Queue->front = Queue->rear;//初始化时队列头队列首相连
    Queue->count = 0;   //队列计数为0
}

// Queue In
uint8_t QueueIn(FifoQueueTypedef *Queue,ElemType sdat) //数据进入队列
{
    if((Queue->front == Queue->rear) && (Queue->count == QueueSize))
    {												 //判断如果队列满了
        return QueueFull;    //返回队列满的标志
    }else
    {
        Queue->dat[Queue->rear] = sdat;
        Queue->rear = (Queue->rear + 1) % QueueSize;
        Queue->count = Queue->count + 1;
        return QueueOperateOk;
    }
}

// Queue Out
uint8_t QueueOut(FifoQueueTypedef *Queue,ElemType *sdat)
{
    if((Queue->front == Queue->rear) && (Queue->count == 0))
    {                    // empty
        return QueueEmpty;
    }else
    {                    // out
        *sdat = Queue->dat[Queue->front];
        Queue->front = (Queue->front + 1) % QueueSize;
        Queue->count = Queue->count - 1;
        return QueueOperateOk;
    }
}