/**
  ******************************************************************************
  * @file    turtle_fifo.c
  * @author  Xucz(OUC Fab U+/ROV Team)
  * @brief   Turtle ϵ�� ROV ��FIFO��غ���
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
    Queue->front = Queue->rear;//��ʼ��ʱ����ͷ����������
    Queue->count = 0;   //���м���Ϊ0
}

// Queue In
uint8_t QueueIn(FifoQueueTypedef *Queue,ElemType sdat) //���ݽ������
{
    if((Queue->front == Queue->rear) && (Queue->count == QueueSize))
    {												 //�ж������������
        return QueueFull;    //���ض������ı�־
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